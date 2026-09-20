from pathlib import Path
import sys

def symlink(dst: Path, src: Path):
    print(f'{dst=} {src=}')
    if dst.is_symlink():
        dst.unlink()
    elif dst.exists():
        raise FileExistsError(f'error: expected {dst} to be a symlink')
    base = Path(*dst.absolute().parts[:-1])
    rel = src.relative_to(base, walk_up=True)
    print(rel)
    dst.symlink_to(rel, target_is_directory=True)

if len(sys.argv) != 2:
    print(f"usage: {sys.argv[0]} <path>")
    sys.exit(1)

path = Path(sys.argv[1])
symlink(Path('src/user'), path / 'src')
symlink(Path('include/user'), path / 'include')

