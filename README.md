# 💡 Relay 8X Controller
> [!WARNING]
> Il seguente codice è realizzato esclusivamente per ESP32 DevKit con relativo circuito assemblato correttamente. Le
> schematiche saranno incluse nel progetto successivamente.

Relay8XController è un progetto realizzato per permettere l'aggiornamento di un impianto luce tradizionale ad uno domotico,
utilizzando un semplicissimo ESP32 applicato ad un circuito relativamente semplice da realizzare.

Esso permette l'applicazione di 8 luci ed 8 ingressi-pulsante per ciascuna delle 8 luci, tutte connesse a dei relay da
10A massimi in 240 AC 50 Hz.

## 🛜 WiFi
Per connettere il tutto alla rete Wireless, sarà necessario impostare un nome di rete e la rispettiva password all'interno
di un file creato nella cartella **src/**, ovvero **secrets.h**. Il contenuto sarà il seguente:

```c++
#define SSID                                "SSID"
#define PASSWORD                            "PASSWORD"
```

Con, ovviamente, i valori personali.
