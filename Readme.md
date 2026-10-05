## Hardware Necessário

| qte | Componente              |
| --- | ----------------------- |
| 1   | ESP32                   |
| 1   | LED                     |
| 1   | Resistor 300 ${\Omega}$ |

### Circuito

LED conectado ao **GPIO 5** do ESP32 (com resistor de 300 Ω em série). Veja o esquemático:

![circuito ESP](./docs/images/circuitoEsp.png)

Ligação típica:

- GPIO 5 → Resistor 300 Ω → Anodo LED (+)
- Catodo LED (−) → GND ESP32

