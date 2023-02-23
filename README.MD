IC-Card-Process-Platform
===

A simple IC Card process platform. Include read UID and write to white card (also called Chinese Magic Card).

## Design

```
                                                                              (with charge Protect)
RFID Module --- Arduino Nano ---                            --- Battery --- DC Boost converter Module  
                                Mode Switch --- OLED Screen                          --- Power Switch
```

![493023](https://user-images.githubusercontent.com/33981880/220870907-3a825185-6277-44b0-a6d9-7a23ae9dda21.jpg)


## Demo

Read-Only Mode

https://user-images.githubusercontent.com/33981880/220870313-cfc7840b-64f1-4948-aa0f-9ca9d1386acc.mp4

Read and write to white card

https://user-images.githubusercontent.com/33981880/220870214-06c1adf9-aa06-42e0-8d60-5e1f1e0b16c2.mp4
