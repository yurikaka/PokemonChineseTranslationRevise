#!/usr/bin/env python3

import argparse
import pathlib
import zipfile


def main() -> None:
    parser = argparse.ArgumentParser(description="Create a NitroPatcher XZP archive.")
    parser.add_argument("source", type=pathlib.Path)
    parser.add_argument("output", type=pathlib.Path)
    args = parser.parse_args()

    source = args.source.resolve()
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with zipfile.ZipFile(args.output, "w", compression=zipfile.ZIP_DEFLATED) as archive:
        for path in sorted(source.rglob("*")):
            if path.is_file():
                archive.write(path, path.relative_to(source).as_posix())


if __name__ == "__main__":
    main()
