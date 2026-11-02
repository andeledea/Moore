# MOORE         {#mainpage}
Programma per controllo assi e misure con la macchina MOORE INRIM.

## Compilazione del sorgente
```
- Make			-> compilazione completa del codice
- Make CXXFLAGS="-DTRY" -> compilazione della sola GUI per testing
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

## NI dlls
Download the libraries from here:
https://www.ni.com/en/support/downloads/drivers/download.ni-488-2.html?srsltid=AfmBOopOVJQ2MIWL5-SFBGaVQMyVz17RniQwAbaWo3XjdJL5Y1YnTpxM#544048

In system32 you should have:
```
-a---          10/15/2024  4:27 PM         384600 ni4882.dll
-a---          10/15/2024  4:27 PM         108648 ni488config.dll
-a---          10/15/2024  4:27 PM         482928 nipalu.dll
-a---          10/15/2024  4:27 PM          22656 nipalut.dll
```

and other libs, add them to lib folder.

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

## lib/gcc_dll folder content
```
d----          10/15/2024 10:18 AM                mswu(folder)
-a---           3/19/2024  9:37 AM         748032 CHRocodileDLL.dll
-a---           3/19/2024  9:37 AM        3573112 E1735A.dll
-a---           3/19/2024  9:37 AM        3718520 E1735ACore.dll
-a---           3/19/2024  9:37 AM        1650552 E1736A.dll
-a---           3/19/2024  9:37 AM        3556728 E1736ACore.dll
-a---           3/19/2024  9:37 AM          59568 gpibconf.dll
-a---           3/19/2024  9:37 AM         175464 IK220Dll.Dll
-a---           3/19/2024  9:37 AM         239392 IK220Dll64.dll
-a---          10/15/2024  9:39 AM         317886 libwxbase32u_net.a
-a---          10/15/2024 10:07 AM          46298 libwxbase32u_xml.a
-a---          10/15/2024  9:38 AM        2710706 libwxbase32u.a
-a---          10/15/2024  9:30 AM         209846 libwxexpat.a
-a---          10/15/2024  9:30 AM         364930 libwxjpeg.a
-a---          10/15/2024 10:04 AM           1822 libwxmsw32u_adv.a
-a---          10/15/2024 10:11 AM         569178 libwxmsw32u_aui.a
-a---          10/15/2024 10:04 AM        9411270 libwxmsw32u_core.a
-a---          10/15/2024 10:15 AM          86972 libwxmsw32u_gl.a
-a---          10/15/2024 10:06 AM         563188 libwxmsw32u_html.a
-a---          10/15/2024 10:05 AM         128786 libwxmsw32u_media.a
-a---          10/15/2024 10:13 AM        1077584 libwxmsw32u_propgrid.a      
-a---          10/15/2024 10:12 AM         470210 libwxmsw32u_ribbon.a        
-a---          10/15/2024 10:15 AM        1713638 libwxmsw32u_richtext.a      
-a---          10/15/2024 10:15 AM         699886 libwxmsw32u_stc.a
-a---          10/15/2024 10:07 AM         116004 libwxmsw32u_webview.a       
-a---          10/15/2024 10:10 AM         627410 libwxmsw32u_xrc.a
-a---          10/15/2024  9:30 AM         291028 libwxpng.a
-a---          10/15/2024  9:29 AM         412922 libwxregexu.a
-a---          10/15/2024  9:33 AM        3511220 libwxscintilla.a
-a---          10/15/2024  9:30 AM         552876 libwxtiff.a
-a---          10/15/2024  9:29 AM         110528 libwxzlib.a
-a---            7/2/2024 10:38 AM         384600 ni4882.dll
-a---           6/28/2024  2:11 PM         108648 ni488config.dll
-a---           3/19/2024  9:37 AM          18944 wsock32.dll
-a---          10/15/2024  9:38 AM        4169125 wxbase32u_gcc_custom.dll    
-a---          10/15/2024  9:39 AM         594844 wxbase32u_net_gcc_custom.dll
-a---          10/15/2024 10:07 AM         317244 wxbase32u_xml_gcc_custom.dll
-a---          10/15/2024 10:04 AM          38360 wxmsw32u_adv_gcc_custom.dll 
-a---          10/15/2024 10:11 AM        1070824 wxmsw32u_aui_gcc_custom.dll 
-a---          10/15/2024 10:04 AM       14179593 wxmsw32u_core_gcc_custom.dll       
-a---          10/15/2024 10:15 AM         253665 wxmsw32u_gl_gcc_custom.dll
-a---          10/15/2024 10:06 AM        1437223 wxmsw32u_html_gcc_custom.dll       
-a---          10/15/2024 10:05 AM         291859 wxmsw32u_media_gcc_custom.dll      
-a---          10/15/2024 10:13 AM        1600285 wxmsw32u_propgrid_gcc_custom.dll   
-a---          10/15/2024 10:12 AM         656199 wxmsw32u_ribbon_gcc_custom.dll     
-a---          10/15/2024 10:15 AM        2760420 wxmsw32u_richtext_gcc_custom.dll   
-a---          10/15/2024 10:15 AM        3507363 wxmsw32u_stc_gcc_custom.dll        
-a---          10/15/2024 10:07 AM         574654 wxmsw32u_webview_gcc_custom.dll    
-a---          10/15/2024 10:10 AM        1772022 wxmsw32u_xrc_gcc_custom.dll 
```


### @andeledea (Andrea Giura), GitHub.com
