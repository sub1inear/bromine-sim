# /// script
# requires-python = ">=3.9"
# dependencies = ["pros-cli", "furl"]
# ///

from pathlib import Path
import sys
import os
import json
from pros.conductor.conductor import Conductor
import requests
from io import BytesIO
import zipfile
from furl import furl
import shutil

def symlink(dst: Path, src: Path):
    dir_path = Path(dst.parent).resolve()
    # Sanity-check we're not deleting anything outside of bromine-sim.
    root = Path(__file__).resolve().parent.parent
    if not dir_path.is_relative_to(root):
        raise ValueError(f'error: {dir_path} is not relative to {root}')
    if src.resolve().is_relative_to(dir_path):
        raise ValueError(f'error: {src} is inside {dir_path}')

    if dst.is_symlink():
        dst.unlink()
    elif dst.exists():
        raise FileExistsError(f'error: expected {dst} to be a symlink')

    if dir_path.exists():
        shutil.rmtree(dir_path)
    dir_path.mkdir(parents=True)
    # os.path.relpath() requires a lower Python version than Path.relative_to().
    rel = os.path.relpath(src, start=dst.parent)
    print(f'creating symlink {rel} -> {dst}')
    dst.symlink_to(rel, target_is_directory=True)

if len(sys.argv) != 2:
    print(f'usage: {sys.argv[0]} <path>')
    sys.exit(1)

path = Path(sys.argv[1])

src = Path('src/user')
include = Path('include/user')

symlink(Path('src/user/src'), path / 'src')
symlink(Path('include/user/include'), path / 'include')

conductor = Conductor()

with open(path / 'project.pros') as f:
    project = json.load(f)
    for name, template in project['py/state']['templates'].items():
        if name in ['kernel', 'liblvgl']:
            continue
        print(f'installing template {name}...')
        origin = template['metadata']['origin']
        version = template['version']

        depot = conductor.get_depot(origin)
        if depot is None:
            raise KeyError(f"error: depot not found for template {name}: {origin}")
        url = depot.location

        response = requests.get(url)
        response.raise_for_status()
        depot_json = json.loads(response.text)

        for depot_template in depot_json:
            depot_name = depot_template['name']
            depot_version = depot_template['version']
            if depot_name == name and depot_version == version:
                location = furl(depot_template['metadata']['location'])
                if location.netloc != 'github.com':
                    raise ValueError(f'error: expected github.com source for template {name}: {location}')
                author = location.path.segments[0]
                repo = location.path.segments[1]
                tag = location.path.segments[4]

                source_code = furl('https://github.com').add(path=[author, repo, 'archive', 'refs', 'tags', f'{tag}.zip'])

                source_code_response = requests.get(source_code.url)
                source_code_response.raise_for_status()

                with zipfile.ZipFile(BytesIO(source_code_response.content)) as zip_file:
                    all_files = zip_file.namelist()
                    files_to_extract = [f for f in all_files if not f.endswith('main.cpp')]
                    zip_file.extractall(path=src, members=files_to_extract)
                print(f'successfully installed template {name} from {source_code}')
                break
        else:
            raise KeyError(f"error: template {name} not found in depot {depot.name} at {url}")

print('success!')
