# MOORE         {#mainpage}
Programma per controllo assi e misure con la macchina MOORE INRIM.

## Compilazione del sorgente
```
- Make			-> compilazione completa del codice
- Make CXXFLAGS="-DGUI" -> compilazione della sola GUI per testing
- Make clean		-> rimozione dell'eseguibile e dei file oggetto
- Make layout		-> modifica del layout GUI
```

## Compilazione GUI library da sorgente
Scaricare la libreria grafica wxWidgets e compilarla da sorgente
[wxWidgets Downloads](https://docs.wxwidgets.org)

```
make -f makefile.gcc BUILD=release SHARED=1 UNICODE=1 clean
make -f makefile.gcc BUILD=release SHARED=1 UNICODE=1
```

Copiare include folder and lib folder nella cartella Moore
prima di compilare.
La cartella lib deve avere la seguente struttura:
```
├───lib
│   └───gcc_dll
│       └───mswu
│           └───wx
│               └───msw
```
gcc_dll deve contenere tutte le librerie grafiche e non che servono per il funzionamento
del programma, le librerie grafiche si ottengono dalla compilazione di wxWidgets.
```
28/07/2023  17:15           748,032 CHRocodileDLL.dll
31/07/2023  17:19            59,568 gpibconf.dll
28/07/2023  17:15           175,464 IK220Dll.Dll
28/07/2023  17:15           239,392 IK220Dll64.dll
31/07/2023  17:19           380,072 ni4882.dll
31/07/2023  17:19            23,704 ni488config.dll

31/07/2023  17:19         7,082,351 wxbase32u_gcc_custom.dll
.
.
.
31/07/2023  17:19         4,713,371 wxmsw32u_xrc_gcc_custom.dll
```

## GUI graphical tool
Per lo sviluppo grafico usare il programma **wxFormBuilder** scaricabile da:
[wxFormBuilder Downloads](https://github.com/wxFormBuilder/wxFormBuilder/releases)
Il file moore.fbp è il progetto grafico, una volta generato il codice usare
```
make layout
```
per distribuire automaticamente i file nelle apposite cartelle.


## Serial library
La libreria è già inclusa nel codice sorgente in quanto si tratta di soli 2 file
[simpleSerial Downloads](https://github.com/dmicha16/simple_serial_port/tree/master)

## uC Firmware
Il firmware del uC (NUCLEO BOARD) deve essere caricato sulla scheda tramite
ST-LINK o STM32CubeIDE

[STM32cube Downloads](https://www.st.com/en/development-tools/stm32cubeide.html#get-software)

E' possibile anche usare STM32MX per la generazione del codice di cofigurazione
delle periferiche del microcontrollore e un diverso ide per la personalizzazione.

[STM32MX Downloads](https://www.st.com/en/development-tools/stm32cubemx.html#get-software)

[STM32Uploader Downloads](https://www.st.com/en/development-tools/stm32cubeprog.html#get-software)

## Documentazione
La documentazione è scritta nella forma:
```
/**
* Brief explanation of the function/method/class
*
* @param a: explanation of the param
* @param b:
* 
* @return ret: explanation of the return value
* 
* @throw Exception: explanation of the failure
* */
```
Il programma *DOXIGEN* permette la generazione automatica della documentazione
lateX e html

[Doxigen downloads](https://www.doxygen.nl/index.html)

## Project folder structure
Come la cartella dovrebbe presentarsi al momento della compilazione
```
Moore
├───bin
├───docs
│   └───html
│       └───search
├───icons
├───include
│   ├───msvc
│   │   └───wx
│   └───wx
├───lib
│   └───gcc_dll
│       └───mswu
│           └───wx
│               └───msw
├───obj
└───src
    ├───GUI_inc
    └───moore_inc
```


### @andeledea (Andrea Giura), GitHub.com
