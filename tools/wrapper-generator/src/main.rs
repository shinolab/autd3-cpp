/*
 * File: main.rs
 * Project: src
 * Created Date: 08/12/2023
 * Author: Shun Suzuki
 * -----
 * Last Modified: 24/01/2024
 * Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
 * -----
 * Copyright (c) 2023 Shun Suzuki. All rights reserved.
 *
 */

use std::env;
use std::path::Path;

use anyhow::Result;
use glob::glob;

pub fn gen_c<P1: AsRef<Path>, P2: AsRef<Path>>(crate_path: P1, dest_dir: P2) -> Result<()> {
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
            "TimerStrategy".to_string(),
            "GainSTMMode".to_string(),
            "ControllerPtr".to_string(),
            "EmissionConstraintPtr".to_string(),
            "FirmwareInfoListPtr".to_string(),
            "GroupKVMapPtr".to_string(),
            "CachePtr".to_string(),
            "DevicePtr".to_string(),
            "TransducerPtr".to_string(),
            "GeometryPtr".to_string(),
            "ModulationPtr".to_string(),
            "GainPtr".to_string(),
            "LinkPtr".to_string(),
            "DatagramPtr".to_string(),
            "DatagramSpecialPtr".to_string(),
            "STMPropsPtr".to_string(),
            "BackendPtr".to_string(),
            "GroupGainMapPtr".to_string(),
            "GainCalcDrivesMapPtr".to_string(),
            "LinkBuilderPtr".to_string(),
            "ResultI32".to_string(),
            "ResultModulation".to_string(),
            "ResultBackend".to_string(),
            "ResultController".to_string(),
            "ResultGainCalcDrivesMap".to_string(),
            "ResultDatagram".to_string(),
            "Drive".to_string(),
        ],
        exclude: vec!["ConstPtr".to_string()],
        rename: vec![
            ("float".to_string(), "double".to_string()),
            ("ConstPtr".to_string(), "void*".to_string()),
        ]
        .into_iter()
        .collect(),
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
        .write_to_file(out_file);

    Ok(())
}

fn main() -> Result<()> {
    let home = env::var("CARGO_MANIFEST_DIR")?;
    for entry in glob(&format!("{}/capi/*/Cargo.toml", home))? {
        let entry = entry?;
        let crate_path = Path::new(&entry).parent().unwrap();
        gen_c(&crate_path, "../../include/autd3/native_methods")?;
    }
    Ok(())
}
