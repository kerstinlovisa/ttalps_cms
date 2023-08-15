
mass=$1
part=$2
subpart=$3

mkdir tmp_configs
mkdir -p output/tta_mAlp-${mass}GeV_nEvents-10000_part-${part}
tmp_config=tmp_configs/skimmer_config_signal_${mass}_${part}_${subpart}.py

sed "s/MASS/$mass/g" configs/skimmer_config_signal.py > $tmp_config
sed -i "s/SUBP/$subpart/g" $tmp_config
sed -i "s/PART/$part/g" $tmp_config

./build/skimmer $tmp_config