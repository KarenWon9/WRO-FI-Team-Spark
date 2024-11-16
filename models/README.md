# 3D desing

## Parts list
| Peace                          | Total    |
|--------------------------------|----------|
| Front ring                     | 2        |
| Back ring                      | 2        |
| Pivot                          | 2        |
| Pivot shaft                    | 2        |
| Ultrasonic case pt1            | 2        |
| Ultrasonic case pt2            | 2        |
| Flexible wings                 | 2        |
| Pixy base                      | 1        |
| Laser sensor case pt1          | 1        |
| Laser sensor case pt2          | 1        |
| Chasis first floor             | 1        |
| Chasis second floor            | 1        |
| Chasis third floor             | 1        |
| Chasis fourth floor pt1        | 1        |
| Chasis fourth floor pt2        | 1        |

## Printing settings and recommendations

In this explanation we use the original printing configurations implemented in the proyect, but there are parameters that depend on the material to be used. In the proyect we use PETG by sunlu, in addition to TPU by Eryone for the flexible parts in the chassis (the wings). You can modify this parameter depending on the material and the temperature of the ambient, the melting point of the material, and your 3D printer potential. in this case, we use the Elegoo Neptune 3 Pro.

All the files are distributed in an optimal positioning; this can say they are in a position ready to print when you insert them in the slicer (this time we use Ultimaker Cura) most of the pieces do not need support, except some pieces like the ultrasonicase pt 2 and the laser sensor pt1, But evaluating the configurations shows why it is recommended to use support in most of the pieces to increase the final resource, there are the recommended configurations for the pieces.

- Layer Height: 2 mm
- Recommended infill density: 35% 60% 
- Infil pattern: triangular (You can change the infill pattern)

- Printing speed: 
    - First layer: 10 mm/s
    - Outer wall 75 mm/s
    - Inner wall 90 mm/s
    - top/Bottom 75 mm/s

- Fan speed
    - regular speed 30%
    - Máximum speed 45%
    - Mínimum speed 10%
    - Initial speed 15%

- Support:
    - Support structure: Normal
    - Support overhang angle: 45°
    - Support pattern: zig zag
    - Support density: 25%-35%
    - Support placement: Touching build plate

- Used printing temperatures: 
    - PETG:
        - Nozzle: 255°
        - Build Plate: 105°
    - TPU
        - Nozzle: 220°
        - Build Plate: 70°

> [!NOTE]
> The only pieces that is totally necessary to print with supports like the Ultrasonic Case pt2 and Laser sensor case pt1, We recommend print the pieces in this configurations.

[![Whats-App-Image-2024-11-11-at-4-53-03-PM-1.jpg](https://i.postimg.cc/mrKMqxyG/Whats-App-Image-2024-11-11-at-4-53-03-PM-1.jpg)](https://postimg.cc/gwqnXT7M)

[![Whats-App-Image-2024-11-11-at-4-53-04-PM.jpg](https://i.postimg.cc/kg1WQRVG/Whats-App-Image-2024-11-11-at-4-53-04-PM.jpg)](https://postimg.cc/HrMJHx91)

> [!TIP]
> The recommended materials for this project are ASA and ABS for their mechanical properties, Temperature resistance and stability for this Kind of projects that requires mechanical stress and impact resistance.
