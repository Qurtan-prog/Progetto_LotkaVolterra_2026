# Progetto_LotkaVolterra_2026


cmake -S . -B build -G"Ninja Multi-Config"
cmake --build build --config Debug
cmake --build build --config Debug --target test
cmake --build build --config Release
cmake --build build --config Release --target test

./build/lotka_volterra