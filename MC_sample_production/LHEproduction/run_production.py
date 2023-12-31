import os
import math
from pathlib import Path
import shutil
import argparse
import fileinput
from math import sqrt, pi
import random
import subprocess

#username = os.getlogin()
# username = "jniedzie"
username = "lrygaard"

if username == "jniedzie":
    python_path = "/afs/desy.de/user/j/jniedzie/miniconda3/envs/tta/bin/python3"
    mg_path = "/afs/desy.de/user/j/jniedzie/MG5_aMC_v3_4_2/bin/mg5_aMC"
    hepmc_path = "/afs/desy.de/user/j/jniedzie/ttalps/ttalps/external_tools/hepmc2root/hepmc2root"
    pythia_path = "/afs/desy.de/user/j/jniedzie/MG5_aMC_v3_4_2/HEPTools/pythia8/share/Pythia8/xmldoc"
elif username == "lrygaard":
    python_path = "/afs/desy.de/user/l/lrygaard/tools/miniconda3/envs/tta/bin/python3"
    mg_path = "/afs/desy.de/user/l/lrygaard/tools/MG5_aMC_v3_4_2/bin/mg5_aMC"
    hepmc_path = "/afs/desy.de/user/l/lrygaard/ALPpheno/ttalps/external_tools/hepmc2root/hepmc2root"
    pythia_path = "/afs/desy.de/user/l/lrygaard/tools/MG5_aMC_v3_4_2/HEPTools/pythia8/share/Pythia8/xmldoc"
else:
    print("Error: unrecognized username")

base_pythia_card = "pythia8_card.dat"
base_param_card = "param_card.dat"

c = 29979245800 # speed of light in cm/s
hbar = 6.582119569*10**(-25) # h/2pi in GeVs

def remove_existing_files(output_path, file_name):
    command = f"rm -rf {output_path}/{file_name}/"
    print(f"running command: {command}")
    process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    process.wait()


def run_madgraph(config_path):
    command = f"{python_path} {mg_path} {config_path}"
    print(f"running command: {command}")
    os.system(command)


def convert_hepmc_to_root(output_path, file_name):
    hepmc_file_path_original = output_path + "/" + file_name + "/Events/run_01/tag_1_pythia8_events.hepmc.gz"
    hepmc_file_path = output_path + "/" + file_name + "/Events/run_01/" + file_name + ".hepmc.gz"

    command = f"mv {hepmc_file_path_original} {hepmc_file_path}"
    os.system(command)

    command = f"cd {output_path}/{file_name}/Events/run_01/;"
    command += f"gzip -d {file_name}.hepmc.gz"
    os.system(command)
    
    command = f"cd {output_path}/{file_name}/Events/run_01/;"
    command += f"{hepmc_path} {file_name}.hepmc"
    os.system(command)


def move_and_cleanup_files(output_path, file_name, file_format):
    
    output_dir_name = "_".join(file_name.split("_")[0:-1])
    
    command = f"mkdir -p {output_path}/{output_dir_name}"
    os.system(command)

    if file_format == "ROOT" or file_format == "BOTH":
        command = f"mkdir -p {output_path}/{output_dir_name}_delphes"
        os.system(command)
    
        command = f"mv {output_path}/{file_name}/Events/run_01/{file_name}.root {output_path}/{output_dir_name}/"
        os.system(command)
   
        command = f"mv {output_path}/{file_name}/Events/run_01/tag_1_delphes_events.root {output_path}/{output_dir_name}_delphes/{file_name}.root"
        os.system(command)
 
    if file_format == "LHE" or file_format == "BOTH":
        command = f"mv {output_path}/{file_name}/Events/run_01/unweighted_events.lhe.gz "
        command += f"{output_path}/{output_dir_name}/{file_name}.lhe.gz"
        os.system(command)
    
    command = f"rm -fr {output_path}/{file_name}/"
    os.system(command)


def run_command(config_path, output_path, file_name, file_format):
    
    remove_existing_files(output_path, file_name)
    run_madgraph(config_path)
    if file_format == "ROOT" or file_format == "BOTH":
        convert_hepmc_to_root(output_path, file_name)
    move_and_cleanup_files(output_path, file_name, file_format)
    
    
def create_paths(paths):
    for path in paths:
        Path(path).mkdir(parents=True, exist_ok=True)

    
def copy_and_update_config(base_config_path, new_config_path, values_to_change):
    
    shutil.copyfile(base_config_path, new_config_path)

    for line in fileinput.input(new_config_path, inplace=True):
        line = line.rstrip()
        if not line:
            continue
        
        for key, value in values_to_change.items():
            keyword, default_value = key
            if keyword in line:
                line = line.replace(default_value, str(value))
        print(line)
        

def get_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-p", "--part", help="", default=0)
    parser.add_argument("-n", "--n_events", help="", default=100)
    parser.add_argument("-m", "--alp_mass", help="", default=50e-03)
    parser.add_argument("-o", "--output_path", help="", default=".")
    parser.add_argument("-pr", "--process", help="", default="tta")
    parser.add_argument("-l", "--lifetime", help="", default=0)
    parser.add_argument("-ff", "--file_format", help="", default="LHE")
    
    return parser.parse_args()


def clear_string(s):
    return s.replace(".", "p").replace("-", "m")
    

def get_output_file_name(process, part, n_events, alp_mass, alp_lifetime):
    alp_mass_name = clear_string(f"{alp_mass}")
    
    file_name = f"{process}"
    
    if process == "tta":
        file_name += f"_mAlp-{alp_mass_name}GeV"
        if(float(alp_lifetime) > 0):
            file_name += f"_ctau-{alp_lifetime}mm"
    file_name += f"_nEvents-{n_events}"
    file_name += f"_part-{part}"
    
    return file_name


def get_gamma_for_lifetime(lifetime):  # in mm
    lifetime /= 10  # convert to cm
    gamma = c * hbar / lifetime
    return gamma


def main():
    random.seed(None)

    os.system(f"export PYTHIA8DATA={python_path}")
    
    args = get_args()
    output_path = args.output_path

    create_paths(("tmp_cards", output_path))
    
    file_hash = random.getrandbits(128)
    new_mg_card_path = f"tmp_cards/mg_card_{file_hash}.txt"
    new_param_card_path = f"tmp_cards/param_card_{file_hash}.dat"

    process = args.process

    file_name = get_output_file_name(process, args.part, args.n_events, args.alp_mass, args.lifetime)

    # prepare MG card
    to_change = {
        ("output", "dummy_value"): output_path+"/"+file_name,
        ("set mass 54", "dummy_value"): args.alp_mass,
        ("set nevents", "dummy_value"): args.n_events,
        ("set iseed", "dummy_value"): random.randint(0, 900000000),
    }
    if args.file_format == "ROOT" or args.file_format == "BOTH":
        new_pythia_card_path = f"tmp_cards/pythia8_card_{file_hash}.dat"
        to_change[("pythia8_card.dat", "pythia8_card.dat")] = new_pythia_card_path

    if float(args.lifetime) <= 0:
        to_change[("lifetime_options", "lifetime_options")] = "compute_widths y0"
    else:
        to_change[("lifetime_options", "lifetime_options")] = new_param_card_path

    base_mg_card = "mg5_card_tta.dat"

    if process == "tta":
        base_mg_card = "mg5_card_tta.dat"
    elif process == "ttj":
        base_mg_card = "mg5_card_ttj.dat"
    elif process == "ttmumu":
        base_mg_card = "mg5_card_ttmumu.dat"
    else:
        print(f"\n\nERROR -- unrecognized process: {process}")
        exit(0)

    copy_and_update_config(base_mg_card, new_mg_card_path, to_change)

    # prepare pythia card
    if args.file_format == "ROOT" or args.file_format == "BOTH":
        to_change = {
            ("HEPMCoutput:file", "dummy_value"): file_name+".hepmc.gz",
        }
        copy_and_update_config(base_pythia_card, new_pythia_card_path, to_change)

    # prepare param card
    alp_mass = float(args.alp_mass)
    lifetime = float(args.lifetime)
    
    if lifetime > 0:
        gamma = get_gamma_for_lifetime(lifetime)

        to_change = {
            ("# my0", "dummy_value"): alp_mass,
            ("DECAY 54", "dummy_value"): gamma,
        }
        print(f"{alp_mass=}, {gamma=}, lifetime= {c* hbar / gamma * 10} mm")
        copy_and_update_config(base_param_card, new_param_card_path, to_change)

    # run production
    run_command(new_mg_card_path, output_path, file_name, args.file_format)


if __name__ == "__main__":
    main()
