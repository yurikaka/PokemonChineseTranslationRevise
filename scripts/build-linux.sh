#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
workspace_root="$(cd -- "$repo_root/.." && pwd)"
if [[ -f "$workspace_root/build-env.sh" ]]; then
  source "$workspace_root/build-env.sh"
fi
cd "$repo_root"

command -v dotnet >/dev/null || { echo "dotnet is required." >&2; exit 1; }
command -v dotnet-script >/dev/null || { echo "dotnet-script is required." >&2; exit 1; }
command -v arm-none-eabi-gcc >/dev/null || { echo "devkitARM is required." >&2; exit 1; }
: "${DEVKITARM:?DEVKITARM is required}"
: "${PCTR_FONT_DIR:?PCTR_FONT_DIR is required}"

target="${1:-Pt}"
if [[ "$target" != "Pt" ]]; then
  echo "Only the Pt target is implemented for the first Linux build." >&2
  exit 2
fi

run_csx() {
  dotnet-script --cache-path "$workspace_root/.local/dotnet-script-cache" "$@"
}

python3 scripts/merge_messages.py
run_csx scripts/copy_textures.csx
run_csx scripts/Pt.csx

dotnet publish -c Release -f net8.0 tools/PCTRTools/PCTRTools.csproj
pctr_tools="tools/PCTRTools/bin/Release/net8.0/publish/PCTRTools"

"$pctr_tools" replace-narc \
  -i original_files/Pt/data/ \
  -n textures/Pt/ \
  -o out/Pt/data/

"$pctr_tools" font \
  -c files/CharTable.txt \
  -i original_files/Pt/data/graphic/pl_font.narc \
  -o out/Pt/data/graphic/pl_font.narc

"$pctr_tools" text-import \
  -c files/CharTable.txt \
  -i original_files/Pt/data/msgdata/pl_msg.narc \
  -t temp/Pt/messages.txt \
  -o out/Pt/data/msgdata/pl_msg.narc

run_csx scripts/pad_text.csx \
  original_files/Pt/data/msgdata/pl_msg.narc \
  out/Pt/data/msgdata/pl_msg.narc

python3 scripts/create_patch.py out/Pt out/Patch-Pt.xzp
echo "Built: $repo_root/out/Patch-Pt.xzp"
