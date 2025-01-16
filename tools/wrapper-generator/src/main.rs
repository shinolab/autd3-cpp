use std::env;
use std::path::Path;

use anyhow::Result;
use glob::glob;

pub fn gen_c<P1: AsRef<Path>, P2: AsRef<Path>>(
    crate_path: P1,
    dest_dir: P2,
    item_types: Vec<cbindgen::ItemType>,
) -> Result<()> {
    let out_file = dest_dir.as_ref().join(format!(
        "{}.h",
        crate_path.as_ref().file_name().unwrap().to_str().unwrap()
    ));
    let mut config = cbindgen::Config::default();
    config.language = cbindgen::Language::Cxx;
    config.pragma_once = true;
    config.autogen_warning = Some(
        "/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */"
            .to_string(),
    );
    config.namespace = Some("autd3::native_methods".to_string());
    config.no_includes = true;
    config.sys_includes = vec!["cstdint".to_string()];
    config.sort_by = cbindgen::SortKey::None;
    config.usize_is_size_t = true;
    config.export = cbindgen::ExportConfig {
        include: vec![
            "Drive".to_string(),
            "Segment".to_string(),
            "LoopBehavior".to_string(),
            "SamplingConfig".to_string(),
            "SyncMode".to_string(),
            "ProcessPriority".to_string(),
            "TimerStrategy".to_string(),
            "DatagramPtr".to_string(),
            "GainPtr".to_string(),
            "DynWindow".to_string(),
            "DynSincInterpolator".to_string(),
            "ResultGain".to_string(),
            "ResultModulation".to_string(),
            "ResultSamplingConfig".to_string(),
            "TimerStrategyTag".to_string(),
            "SpinStrategyTag".to_string(),
            "TimerStrategyWrap".to_string(),
            "ResultStatus".to_string(),
            "ResultSyncLinkBuilder".to_string(),
            "ResultLinkBuilder".to_string(),
            "ControllerPtr".to_string(),
            "TransitionModeWrap".to_string(),
            "TransducerPtr".to_string(),
            "DevicePtr".to_string(),
            "GeometryPtr".to_string(),
            "LinkPtr".to_string(),
            "DebugTypeWrap".to_string(),
            "FociSTMPtr".to_string(),
            "GainSTMPtr".to_string(),
            "GPIOIn".to_string(),
            "GPIOOut".to_string(),
            "SilencerTarget".to_string(),
            "GainSTMMode".to_string(),
            "ResultFociSTM".to_string(),
            "ResultGainSTM".to_string(),
            "ControllerBuilderPtr".to_string(),
            "DcSysTime".to_string(),
            "Duration".to_string(),
            "OptionDuration".to_string(),
        ],
        exclude: vec!["ConstPtr".to_string()],
        rename: vec![("ConstPtr".to_string(), "const void*".to_string())]
            .into_iter()
            .collect(),
        item_types,
        ..Default::default()
    };
    config.function = cbindgen::FunctionConfig {
        sort_by: None,
        must_use: Some("[[nodiscard]]".to_string()),
        ..Default::default()
    };
    config.constant = cbindgen::ConstantConfig {
        allow_static_const: false,
        allow_constexpr: true,
        sort_by: Some(cbindgen::SortKey::None),
    };

    cbindgen::Builder::new()
        .with_crate(crate_path)
        .with_config(config)
        .generate()?
        .write_to_file(&out_file);

    let content = std::fs::read_to_string(&out_file)?;

    let re = regex::Regex::new(r"constexpr const float (.*) = ([\d\.]+);").unwrap();
    let content = re
        .replace_all(&content, "constexpr const float $1 = ${2}f;")
        .to_string();

    let re = regex::Regex::new(r"FfiFuture<(.*)>").unwrap();
    let content = re.replace_all(&content, "FfiFuture$1").to_string();

    std::fs::write(&out_file, content)?;

    Ok(())
}

fn main() -> Result<()> {
    let home = env::var("CARGO_MANIFEST_DIR")?;
    for entry in glob(&format!("{}/autd3/*/Cargo.toml", home))? {
        let entry = entry?;
        let crate_path = Path::new(&entry).parent().unwrap();
        if crate_path.file_name() == Some("autd3-core".as_ref()) {
            gen_c(
                &crate_path,
                "../../include/autd3/native_methods",
                vec![cbindgen::ItemType::Enums, cbindgen::ItemType::Structs],
            )?;
        }
    }
    for entry in glob(&format!("{}/capi/*/Cargo.toml", home))? {
        let entry = entry?;
        let crate_path = Path::new(&entry).parent().unwrap();
        gen_c(
            &crate_path,
            "../../include/autd3/native_methods",
            vec![
                cbindgen::ItemType::Constants,
                cbindgen::ItemType::Enums,
                cbindgen::ItemType::Unions,
                cbindgen::ItemType::Structs,
                cbindgen::ItemType::Functions,
            ],
        )?;
    }
    Ok(())
}
