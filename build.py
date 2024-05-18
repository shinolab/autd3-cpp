#!/usr/bin/env python3

import argparse
import contextlib
import os
import pathlib
import platform
import re
import shutil
import subprocess
import sys
import tarfile
import urllib.request
from glob import glob
from typing import List, Optional


def fetch_submodule():
    if shutil.which("git") is not None:
        with working_dir(os.path.dirname(os.path.abspath(__file__))):
            subprocess.run(
                ["git", "submodule", "update", "--init", "--recursive"]
            ).check_returncode()
    else:
        err("git is not installed. Skip fetching submodules.")


def generate_wrapper():
    if shutil.which("cargo") is not None:
        with working_dir(os.path.dirname(os.path.abspath(__file__))):
            with working_dir("tools/wrapper-generator"):
                subprocess.run(
                    [
                        "cargo",
                        "run",
                    ]
                ).check_returncode()
    else:
        err("cargo is not installed. Skip generating wrapper.")


def err(msg: str):
    print("\033[91mERR \033[0m: " + msg)


def warn(msg: str):
    print("\033[93mWARN\033[0m: " + msg)


def info(msg: str):
    print("\033[92mINFO\033[0m: " + msg)


def rm_f(path):
    try:
        os.remove(path)
    except FileNotFoundError:
        pass


def onexc(func, path, exeption):
    import stat

    if not os.access(path, os.W_OK):
        os.chmod(path, stat.S_IWUSR)
        func(path)
    else:
        raise


def rmtree_f(path):
    try:
        shutil.rmtree(path, onerror=onexc)
    except FileNotFoundError:
        pass


@contextlib.contextmanager
def working_dir(path):
    cwd = os.getcwd()
    os.chdir(path)
    try:
        yield
    finally:
        os.chdir(cwd)


class Config:
    _platform: str
    release: bool
    no_examples: bool
    cmake_extra: Optional[List[str]]
    arch: str

    def __init__(self, args):
        self._platform = platform.system()

        if not self.is_windows() and not self.is_macos() and not self.is_linux():
            err(f'platform "{platform.system()}" is not supported.')
            sys.exit(-1)

        self.release = hasattr(args, "release") and args.release
        self.no_examples = hasattr(args, "no_examples") and args.no_examples
        self.cmake_extra = (
            args.cmake_extra.split(" ")
            if hasattr(args, "cmake_extra") and args.cmake_extra is not None
            else None
        )
        if hasattr(args, "arch") and args.arch is not None:
            self.arch = args.arch
        elif platform.machine().lower() in ["amd64", "x86_64"]:
            self.arch = "x64"
        elif platform.machine().lower() in ["arm64", "aarch64"]:
            self.arch = "arm64"
        else:
            err(f"Unsupported platform: {platform.machine()}")

    def is_windows(self):
        return self._platform == "Windows"

    def is_macos(self):
        return self._platform == "Darwin"

    def is_linux(self):
        return self._platform == "Linux"

    def exe_ext(self):
        return ".exe" if self.is_windows() else ""

    def is_cuda_available(self):
        return shutil.which("nvcc") is not None

    def is_pcap_available(self):
        if not self.is_windows():
            return True
        wpcap_exists = os.path.isfile(
            "C:\\Windows\\System32\\wpcap.dll"
        ) and os.path.isfile("C:\\Windows\\System32\\Npcap\\wpcap.dll")
        packet_exists = os.path.isfile(
            "C:\\Windows\\System32\\Packet.dll"
        ) and os.path.isfile("C:\\Windows\\System32\\Npcap\\Packet.dll")

        return wpcap_exists and packet_exists


def should_update_lib(config: Config, version: str) -> bool:
    if config.is_windows():
        if not os.path.isfile("lib/autd3capi.lib"):
            return True
    else:
        if not os.path.isfile("lib/libautd3capi.a"):
            return True

    if not os.path.isfile("VERSION"):
        return True

    with open("VERSION", "r") as f:
        old_version = f.read().strip()

    return old_version != version


def copy_lib(config: Config):
    with open("include/autd3.hpp", "r") as f:
        content = f.read()
        version = (
            re.search(r"static inline std::string version = \"(.*)\";", content)
            .group(1)
            .split(".")
        )
        if version[2].endswith("-rc"):
            version = ".".join(version)
        else:
            version = ".".join(version[:3])

    if not should_update_lib(config, version):
        return

    if config.is_windows():
        if config.arch == "x64":
            url = f"https://github.com/shinolab/autd3-capi/releases/download/v{version}/autd3-v{version}-win-x64-static.zip"
        elif config.arch == "arm64":
            url = f"https://github.com/shinolab/autd3-capi/releases/download/v{version}/autd3-v{version}-win-arm-static.zip"
        else:
            err(f"Unsupported platform: {platform.machine()}")
            sys.exit(-1)
        urllib.request.urlretrieve(url, "tmp.zip")
        shutil.unpack_archive("tmp.zip", ".")
        rm_f("tmp.zip")
    elif config.is_macos():
        url = f"https://github.com/shinolab/autd3-capi/releases/download/v{version}/autd3-v{version}-macos-aarch64-static.tar.gz"
        urllib.request.urlretrieve(url, "tmp.tar.gz")
        with tarfile.open("tmp.tar.gz", "r:gz") as tar:
            tar.extractall()
        rm_f("tmp.tar.gz")
    elif config.is_linux():
        url = f"https://github.com/shinolab/autd3-capi/releases/download/v{version}/autd3-v{version}-linux-x64-static.tar.gz"
        urllib.request.urlretrieve(url, "tmp.tar.gz")
        with tarfile.open("tmp.tar.gz", "r:gz") as tar:
            tar.extractall()
        rm_f("tmp.tar.gz")
    rmtree_f("bin")

    with open("VERSION", mode="w") as f:
        f.write(version)


def cpp_build(args):
    config = Config(args)

    copy_lib(config)

    if not config.no_examples:
        info("Building examples...")
        with working_dir("examples"):
            os.makedirs("build", exist_ok=True)
            with working_dir("build"):
                command = ["cmake", "..", "-DAUTD_LOCAL_TEST=ON"]
                if config.cmake_extra is not None:
                    for cmd in config.cmake_extra:
                        command.append(cmd)
                if config.is_windows() and config.arch == "arm64":
                    command.append("-A")
                    command.append("ARM64")
                if config.is_windows() and hasattr(args, "vs") and args.vs is not None:
                    if args.vs == "2022":
                        command.append("-GVisual Studio 17 2022")
                    else:
                        err(f"Unsupported Visual Studio version: {args.vs}")
                        sys.exit(-1)
                subprocess.run(command).check_returncode()
                command = ["cmake", "--build", "."]
                if config.release:
                    command.append("--config")
                    command.append("Release")
                subprocess.run(command).check_returncode()


def cpp_test(args):
    args.no_examples = True
    cpp_build(args)

    config = Config(args)
    with working_dir("tests"):
        os.makedirs("build", exist_ok=True)
        with working_dir("build"):
            command = ["cmake", ".."]
            if config.cmake_extra is not None:
                for cmd in config.cmake_extra:
                    command.append(cmd)
            if config.is_cuda_available():
                command.append("-DRUN_BACKEND_CUDA=ON")
            subprocess.run(command).check_returncode()
            command = ["cmake", "--build", ".", "--parallel", "8"]
            if config.release:
                command.append("--config")
                command.append("Release")
            subprocess.run(command).check_returncode()

            target_dir = "."
            if config.is_windows():
                target_dir = "Release" if config.release else "Debug"
            subprocess.run(
                [f"{target_dir}/test_autd3{config.exe_ext()}"]
            ).check_returncode()


def check_if_all_native_methods_called():
    defined_methods = set()
    pattern = re.compile(".* (AUTD.*)\\(.*")
    for file in glob("include/autd3/native_methods/autd3capi*.h"):
        with open(file, "r") as f:
            for line in f.readlines():
                result = pattern.match(line)
                if result:
                    defined_methods.add(result.group(1))
    defined_methods = set(filter(lambda x: not x.endswith("T4010A1"), defined_methods))
    defined_methods.remove("AUTDModulationFourierNearest")

    used_methods = set()
    pattern = re.compile(r".*(AUTD.*?)[\(|,|\)].*")
    for file in glob("include/**/*.hpp", recursive=True) + glob(
        "tests/**/*.cpp", recursive=True
    ):
        with open(file, "r") as f:
            for line in f.readlines():
                result = pattern.match(line)
                if result:
                    used_methods.add(result.group(1))

    unused_methods = defined_methods.difference(used_methods)
    if len(unused_methods) > 0:
        err("Following native methods are defined but not used.")
        for method in unused_methods:
            print(f"\t{method}")
        sys.exit(-1)


def check_all_headers_is_tested():
    with working_dir("include/autd3"):
        headers = glob("**/*.hpp", recursive=True)
        headers = [
            header for header in headers if not header.startswith("native_methods")
        ]
        headers.remove("exception.hpp")
        headers.remove("def.hpp")
        headers.remove("driver/link.hpp")
        headers.remove("driver/datagram/datagram.hpp")
        headers.remove("driver/datagram/gain/base.hpp")
        headers.remove("driver/datagram/gain/gain.hpp")
        headers.remove("driver/datagram/modulation/base.hpp")
        headers.remove("driver/datagram/modulation/modulation.hpp")
        headers.remove("gain/holo.hpp")
        headers.remove("gain/holo/holo.hpp")
        headers.remove("gain/holo/backend_cuda.hpp")
        headers.remove("gain/holo/backend.hpp")
        headers.remove("modulation/audio_file.hpp")
        headers = set([header.replace(".hpp", ".cpp") for header in headers])

    def load_sources(base_path: pathlib.Path) -> set[str]:
        tested = set()
        with open(base_path / "CMakeLists.txt", "r") as f:
            for line in f.readlines():
                subdir = re.search(r"add_subdirectory\((.*)\)", line)
                if subdir:
                    tested = tested | load_sources(base_path / subdir.group(1))

        with open(base_path / "CMakeLists.txt", "r") as f:
            sources = False
            for line in f.readlines():
                if line.startswith("target_sources(test_autd3 PRIVATE"):
                    sources = True
                if not sources:
                    continue
                if line.startswith(")"):
                    break
                src = re.search(r"\s*(.*.cpp)", line)
                if src:
                    tested.add(f"{base_path}/{src.group(1)}")
        return tested

    with working_dir("tests"):
        base_path = pathlib.Path(".")
        tested = load_sources(base_path)

        untested_headers = headers.difference(tested)
        if len(untested_headers) > 0:
            err("Following headers are not tested.")
            for header in sorted(untested_headers):
                print(f"\t{header}")
            sys.exit(-1)

        unincluded_headers = set()
        for cpp in tested:
            hpp = cpp.replace(".cpp", ".hpp")
            with open(base_path / cpp, "r") as f:
                found_include = False
                for line in f.readlines():
                    if re.search(rf"#include <autd3/{hpp}>", line):
                        found_include = True
                        break
                if not found_include:
                    unincluded_headers.add(
                        f"{cpp} does not include target header file {hpp}"
                    )
        if len(unincluded_headers) > 0:
            err("Following source files do not include target header file.")
            for header in sorted(unincluded_headers):
                print(f"\t{header}")
            sys.exit(-1)

        empty_test = set()
        for cpp in tested:
            with open(base_path / cpp, "r") as f:
                found_test = False
                for line in f.readlines():
                    if re.search(r"TEST\(", line):
                        found_test = True
                        break
                if not found_test:
                    empty_test.add(cpp)
        if len(empty_test) > 0:
            err("Following source files do not have any test.")
            for header in sorted(empty_test):
                print(f"\t{header}")
            sys.exit(-1)


def cpp_cov(args):
    check_if_all_native_methods_called()
    check_all_headers_is_tested()

    config = Config(args)
    if not config.is_linux():
        err("Coverage is only supported on Linux.")
        return

    args.no_examples = True
    cpp_build(args)

    with working_dir("tests"):
        os.makedirs("build", exist_ok=True)
        with working_dir("build"):
            command = ["cmake", "..", "-DCOVERAGE=ON"]
            if config.cmake_extra is not None:
                for cmd in config.cmake_extra:
                    command.append(cmd)
            if config.is_cuda_available():
                command.append("-DRUN_BACKEND_CUDA=ON")
            subprocess.run(command).check_returncode()
            command = ["cmake", "--build", ".", "--parallel", "8"]
            if config.release:
                command.append("--config")
                command.append("Release")
            subprocess.run(command).check_returncode()

            with working_dir("CMakeFiles/test_autd3.dir"):
                subprocess.run(
                    [
                        "lcov",
                        "-c",
                        "-i",
                        "-d",
                        ".",
                        "-o",
                        "coverage.baseline",
                    ]
                ).check_returncode()

            target_dir = "."
            if config.is_windows():
                target_dir = "Release" if config.release else "Debug"
            subprocess.run(
                [f"{target_dir}/test_autd3{config.exe_ext()}"]
            ).check_returncode()

            with working_dir("CMakeFiles/test_autd3.dir"):
                subprocess.run(
                    [
                        "lcov",
                        "-c",
                        "-d",
                        ".",
                        "-o",
                        "coverage.out",
                    ]
                ).check_returncode()
                subprocess.run(
                    [
                        "lcov",
                        "-a",
                        "coverage.baseline",
                        "-a",
                        "coverage.out",
                        "-o",
                        "coverage.raw.info",
                    ]
                ).check_returncode()

                subprocess.run(
                    [
                        "lcov",
                        "-r",
                        "coverage.raw.info",
                        "*/_deps/*",
                        "*/usr/*",
                        "*/tests/*",
                        "*/gain/holo/backend_cuda.hpp",
                        "--ignore-errors",
                        "unused",
                        "-o",
                        "coverage.info",
                    ]
                ).check_returncode()
                if args.html:
                    subprocess.run(
                        [
                            "genhtml",
                            "-o",
                            "html",
                            "--num-spaces",
                            "4",
                            "coverage.info",
                        ]
                    ).check_returncode()


def cpp_run(args):
    args.no_examples = False
    cpp_build(args)

    config = Config(args)
    if config.is_windows():
        target_dir = "Release" if args.release else "Debug"
    else:
        target_dir = "."

    subprocess.run(
        [f"examples/build/{target_dir}/{args.target}{config.exe_ext()}"]
    ).check_returncode()


def cpp_clear(_):
    with working_dir("."):
        rmtree_f("lib")
        rmtree_f("bin")
        rmtree_f("examples/build")
        rmtree_f("tests/build")


def util_update_ver(args):
    version = args.version

    with working_dir("."):
        with open("CMakeLists.txt", "r") as f:
            content = f.read()
            version_cmake = version.split(".")
            if version_cmake[2].endswith("-rc"):
                version_cmake[2] = version_cmake[2].replace("-rc", "")
                version_cmake = ".".join(version_cmake[:3])
            else:
                version_cmake = ".".join(version_cmake)
            content = re.sub(
                r"^project\(autd3 VERSION (.*)\)",
                f"project(autd3 VERSION {version_cmake})",
                content,
                flags=re.MULTILINE,
            )
        with open("CMakeLists.txt", "w") as f:
            f.write(content)

        with open("include/autd3.hpp", "r") as f:
            content = f.read()
            content = re.sub(
                r'^static inline std::string version = "(.*)";',
                f'static inline std::string version = "{version}";',
                content,
                flags=re.MULTILINE,
            )
        with open("include/autd3.hpp", "w") as f:
            f.write(content)

        with open("examples/CMakeLists.txt", "r") as f:
            content = f.read()
            content = re.sub(
                r"v.*/autd3-v.*-(win|macos|linux)",
                rf"v{version}/autd3-v{version}-\1",
                content,
                flags=re.MULTILINE,
            )
        with open("examples/CMakeLists.txt", "w") as f:
            f.write(content)


def util_gen_wrapper(_):
    fetch_submodule()
    generate_wrapper()


def command_help(args):
    print(parser.parse_args([args.command, "--help"]))


if __name__ == "__main__":
    with working_dir(os.path.dirname(os.path.abspath(__file__))):
        parser = argparse.ArgumentParser(description="autd3 library build script")
        subparsers = parser.add_subparsers()

        # build
        parser_cpp_build = subparsers.add_parser("build", help="see `build -h`")
        parser_cpp_build.add_argument(
            "--release", action="store_true", help="release build"
        )
        parser_cpp_build.add_argument(
            "--no-examples", action="store_true", help="skip building examples"
        )
        parser_cpp_build.add_argument("--cmake-extra", help="cmake extra args")
        parser_cpp_build.add_argument("--arch", help="architecture (x64, arm64)")
        parser_cpp_build.add_argument("--vs", help="Visual Studio version")
        parser_cpp_build.set_defaults(handler=cpp_build)

        # test
        parser_cpp_test = subparsers.add_parser("test", help="see `test -h`")
        parser_cpp_test.add_argument(
            "--release", action="store_true", help="release build"
        )
        parser_cpp_test.add_argument("--cmake-extra", help="cmake extra args")
        parser_cpp_test.set_defaults(handler=cpp_test)

        # cov
        parser_cpp_cov = subparsers.add_parser("cov", help="see `cov -h`")
        parser_cpp_cov.add_argument(
            "--release", action="store_true", help="release build"
        )
        parser_cpp_cov.add_argument(
            "--html", action="store_true", help="generate html report", default=False
        )
        parser_cpp_cov.add_argument("--cmake-extra", help="cmake extra args")
        parser_cpp_cov.set_defaults(handler=cpp_cov)

        # run
        parser_cpp_run = subparsers.add_parser("run", help="see `run -h`")
        parser_cpp_run.add_argument("target", help="binary target")
        parser_cpp_run.add_argument(
            "--release", action="store_true", help="release build"
        )
        parser_cpp_run.set_defaults(handler=cpp_run)

        # clear
        parser_cpp_clear = subparsers.add_parser("clear", help="see `clear -h`")
        parser_cpp_clear.set_defaults(handler=cpp_clear)

        # util
        parser_util = subparsers.add_parser("util", help="see `util -h`")
        subparsers_util = parser_util.add_subparsers()

        # util update version
        parser_util_upver = subparsers_util.add_parser(
            "upver", help="see `util upver -h`"
        )
        parser_util_upver.add_argument("version", help="version")
        parser_util_upver.set_defaults(handler=util_update_ver)

        # util update version
        parser_util_gen_wrap = subparsers_util.add_parser(
            "gen_wrap", help="see `util gen_wrap -h`"
        )
        parser_util_gen_wrap.set_defaults(handler=util_gen_wrapper)

        # help
        parser_help = subparsers.add_parser("help", help="see `help -h`")
        parser_help.add_argument("command", help="command name which help is shown")
        parser_help.set_defaults(handler=command_help)

        args = parser.parse_args()
        if hasattr(args, "handler"):
            args.handler(args)
        else:
            parser.print_help()
