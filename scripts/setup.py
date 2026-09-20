from pathlib import Path
import sys
import os

def symlink(dst: Path, src: Path):
    if dst.is_symlink():
        dst.unlink()
    elif dst.exists():
        raise FileExistsError(f'error: expected {dst} to be a symlink')
    base = Path(*dst.absolute().parts[:-1])
    # os.path.relpath() requires a lower Python version than Path.relative_to().
    rel = os.path.relpath(src, start=base)
    dst.symlink_to(rel, target_is_directory=True)

if len(sys.argv) != 2:
    print(f"usage: {sys.argv[0]} <path>")
    sys.exit(1)

path = Path(sys.argv[1])
symlink(Path('src/user'), path / 'src')
symlink(Path('include/user'), path / 'include')
print("success!")
