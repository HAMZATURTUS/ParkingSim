## Parking Sim

i plan to use this for a machine learning project as soon as it's ready

An OOD project that simulates a car in a parking lot in 2d.

Parking features will be implemented.... eventually

Physics are mostly based on the equations in [Car Physics for Games](https://www.asawicki.info/Mirror/Car%20Physics%20for%20Games/Car%20Physics%20for%20Games.html)

Each car's measurements are based on a chosen real counterpart and uses their practical values

- GLUTSedan - Toyota Camry
- GLUTHatch - Toyota Yaris
- GLUTSuv - Ford Explorer
- GLUTMinivan - Dodge Caravan
- GLUTVan - GMC Savana

so far the program can only run on linux using

```bash
sudo apt-get install freeglut3-dev
./rerun.sh
```

it can get jittery on wsl but is working fine for me on a debian virtual machine

length, width of cars are based on the reference images

measurements are taken as follows

<img width="1194" height="744" alt="image" src="https://github.com/user-attachments/assets/097b31bb-6a40-4732-9131-5caf141b54a7" />

