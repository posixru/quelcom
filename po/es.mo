��    �      �  �   �	  *     2   3     f  !   �  (   �     �     �      �  "        <  !   [  5   }     �     �     �     �     
          +     <     N  &   `  S   �  T   �  B   0  P   s  Q   �  ,     7   C  I   {  D   �  8   
  C   C  (   �  /   �  V   �  W   7  >   �  &   �  A   �  M   7  6   �  S   �  T     5   e  !   �  2   �  '   �       7   3  B   k     �  A   �  L     #   Z  %   ~  -   �  9   �  -     9   :  P   t  *   �     �  !     5   -     c  /   �  /   �     �  5   �     5     J     _     ~  $   �  '   �     �                    =  
   P  7   [     �      �  '   �     �       "   ,     O  "   o  :   �  .   �     �       "   +  +   N  0   z     �     �  .   �       1        O  "   `  6   �     �  	   �  /   �  0        A  "   I     l       (   �  '   �  !   �     �           "      <      T      k      �      �      �      �      �      !     !     .!     B!     U!     c!     ~!     �!     �!     �!  #   �!     �!     "     "     0"  =   L"  )   �"  )   �"     �"     �"     #     #     5#     D#     V#     t#  1   �#  8   �#  %   �#  (   $  #   H$     l$  $   y$  '   �$  ,   �$  %   �$  (   %  =   B%  %   �%     �%     �%     �%     �%     �%     &     &     $&  &   @&  W   g&  Z   �&  D   '  U   _'  W   �'  4   (  =   B(  Q   �(  R   �(  ?   %)  T   e)  -   �)  8   �)  _   !*  b   �*  D   �*  $   )+  F   N+  P   �+  9   �+  ]    ,  _   ~,  E   �,  !   $-  8   F-  *   -     �-  <   �-  F   .     I.  G   f.  \   �.  )   /  (   5/  1   ^/  :   �/  /   �/  ?   �/  V   ;0  4   �0     �0  '   �0  >   1  &   K1  /   r1  /   �1     �1  2   �1     $2     ;2  )   R2  )   |2  $   �2  (   �2     �2     3     3  "   )3     L3     ^3  ?   j3  "   �3  %   �3  /   �3  !   #4     E4     b4     �4  #   �4  ;   �4  ;   5     >5     Y5  (   x5  -   �5  1   �5     6     6  7   66     n6  -   �6     �6  (   �6  <   �6     ,7     F7  ?   S7  =   �7     �7  &   �7      8  	   8  0   &8  $   W8  *   |8  "   �8  	   �8     �8     �8  !   9  %   '9     M9     ]9     {9     �9     �9     �9     �9     :     !:     <:  #   K:  !   o:     �:     �:     �:  $   �:     �:  +   ;     @;      Y;  ?   z;  -   �;  -   �;     <     /<     H<     [<     v<     �<  "   �<     �<  �   �   )           H   /   ^   w       {   �   �   5       �   �   �   �   T           �   C   	   %   $   ;   `   L   ?   r       |   <   v   y   k       ]   �   �   9       ~      �       R      z      P   J   t           U   O                           #           �   @       S      -   2           G              q   &                      s   E      �      �       I           u   V          f   �   �          _       Q              X   �   M   i           �   B   g   n      
   d   �       h   W       �   7          !   }   c   �       �       >   (   �   �      p   �   l   [          F   8   "      N      +           1   6       ,   \   4       .      A   Y       e      K          Z       �       m      =          D          *   '          0   j                 x   3               o   a   :   b   		 bytes/sample field should be 1, 2 or 4
 		channels field should be 1 (mono) or 2 (stereo)
 		data field should be 'data'
 		data length field should be %d
 		don't know which value must be set...
 		fixed
 		fmt  field should be 'fmt '
 		fmt length field should be %d
 		format field should 1 (pcm tag)
 		riff field should be 'RIFF'
 		riff length field should be %d
 		sample rate field should be between 8000 and 48000
 		wave field should be 'WAVE'
 	bits/sample: %d
 	bytes/sample: %d
 	bytes/second: %d
 	channels: %d
 	data length: %d
 	fmt length: %d
 	riff length: %d
 	sample rate: %d
    -A, --show-all: implies -d, -f, -s
    -B, --set-begin <cut point>: set the first frame from the beginning of the file
    -B, --set-begin <cut point>: set the first sample from the beginning of the file
    -D, --delete: delete invalid frames and garbage. use with care
    -E, --set-end <cut point>: set the last frame from the beginning of the file
    -E, --set-end <cut point>: set the last sample from the beginning of the file
    -F, --fix: correct header. use with care
    -H, --html: output in html format (default is text)
    -S, --shrink: shrink silences greater than 'duration'. use with care.
    -S, --slice <time spec>-<time spec>: set the cut as a time slice
    -S, --split: split report across visited directories
    -T, --delete-tag: delete tag (if exists). option -D must be set
    -V, --version: show version and exit
    -a, --all: report all files, not just *.mp3
    -b, --set-begin-from-eof <cut point>: set the first frame from the end of the file
    -b, --set-begin-from-eof <cut point>: set the first sample from the end of the file
    -c, --check: check the entire stream (slower but accurate)
    -d, --delete: deletes the fragment
    -d, --duration <duration>[j|s|m|b|k|M]: set the fade duration
    -d, --duration <duration>[j|s|m|b|k|M]: set the minimum silence duration.
    -d, --show-dirs: show a report for every directory
    -e, --set-end-from-eof <cut point>: set the last frame from the end of the file
    -e, --set-end-from-eof <cut point>: set the last sample from the end of the file
    -f, --force: force join bypassing bit rate checks
    -f, --fragment [h:[m:]]s[ms] 
    -f, --show-files: show a report for every file
    -h, --help: show this help and exit
    -i, --in: just fade in
    -l, --length [[h:]m:]s[.ms]: set the fade duration.
    -l, --length [[h:]m:]s[.ms]: set the minimum silence duration.
    -o, --out: just fade out
    -o, --output <file>: output file where the fragment is stored
    -o, --output <file>: send output to <file>. otherwise, append to <file1>
    -q, --quiet: no output messages
    -r, --recursive: scan directories
    -s, --show-summary: show a summary report
    -s, --size <cut point>: specifies the size of the cut
    -s, --summary-only: show only the summary
    -t, --test: don't modify. create and fade a test file
    -t, --threshold <threshold>: set the maximum value (in %) for silent samples
    -v, --verbose: show more detailed info
    -v, --verbose: verbose
  %s: check and clean mp3 streams
  %s: detect and shrink silence sequence in wav files
  %s: dump (and fix) wav header
  %s: extract and/or delete parts of a mp3 file
  %s: extract and/or delete parts of a wav file
  %s: fade in/out wav files
  %s: generate reports from mp3 files and directories
  %s: join mp3 files
  %s: join wav files
  %s: show info from mp3 files
  %s: show info from wav files
  <cut point> = <value>[j|s|m|b|k|M]
  => %d:%02d.%03d  %u samples  %u bytes
  bytes of garbage at the end  directories    files    is a special file. skipping.  is not compatible  options:
  see info manual for more info about options and usage
  syntax: %s [option]... file
  syntax: %s [option]... file...
  syntax: %s [option]... file1 file2...
 %d error files found
 %s - version %s - build %s
 %s: %d bytes of garbage at the end %s: %d of %d discardable bytes
 %s: %d:%02d of silence at %d:%02d
 %s: %d:%02d.%03d (%d samples) at %d:%02d:%03d (sample %d)
 %s: '%s' has not enough room for a wav header
 %s: '%s' is a directory
 %s: cannot scan '%s'
 %s: error reading '%s' status: %s
 %s: first frame incomplete: %d of %d bytes
 %s: last frame (%d+1) truncated: %d of %d bytes
 %s: moving %d bytes %s: moving tag (%d bytes) %s: option '%s' is not recognized or bad used
 %s: skipped %d bytes %s: skipped %d bytes of garbage at the beginning
 %s: skipping tag %s: threshold must be 0..100 (%d)
 %s: total silence reported: %d:%02d.%02d (%d samples)
 %s: truncating to %d bytes
 %u file%c : at least two files to join must be specified
 : both fades overlap. they'd be done separately
 : empty : flag -T requires -D. ignoring -T : no input file(s) : no tag : only one file to cut must be specified : smaller than the silence searched for : vbr detected => automatic check : vbr format is not supported [no tag] appending '%s' to '%s'... copying '%s' to '%s'... created testfile '%s'
 cut: last sample > first sample error reading:  faded in %d samples in '%s'
 faded out %d samples in '%s'
 first frame incomplete format is undefined format not recognized:  frame out of range:  header format error header value error invalid field invalid format specifier:  invalid time specification last frame > first frame layer not supported length mismatch move %d..%d (%d samples) to %d..%d
 no '-' found no valid header found not a regular file only layer III is supported options (-b or -B), (-e or -E) and -s cannot be used together options -B and -b cannot be used together options -E and -e cannot be used together quelcom internal error sample out of range:  scanning '%s'... shrink %d samples at %d
 tag is invalid undefined version unsupported bits/sample value unsupported channel value 		 el campo bytes/muestra tiene que ser 1, 2 o 4
 		el campo channels tiene que ser 1 (mono) o 2 (stereo)
 		el campo data tiene que ser 'data'
 		el campo data length tiene que ser %d
 		no s� qu� valor hay que poner...
 		corregido
 		el campo fmt tiene que ser 'fmt '
 		el campo fmt length tiene que ser %d
 		el campo format tiene que ser 1 (pcm tag)
 		el campo riff tiene que ser 'RIFF'
 		el campo riff length tiene que ser %d
 		la velocidad de mostreo tiene que estar entre 8000 y 48000
 		el campo wave tiene que ser 'WAVE'
 	bits/muestra: %d
 	bytes/muestra: %d
 	bytes/segundo: %d
 	canales: %d
 	data length: %d
 	fmt length: %d
 	riff length: %d
 	velocidad de muestreo: %d
    -A, --show-all: implica -d, -f, -s
    -B, --set-begin <punto de corte>: marca el primer frame desde el inicio del fichero
    -B, --set-begin <punto de corte>: marca la primera muestra desde el inicio del fichero
    -D, --delete: borra frames no v�lidos y basura. usar con cuidado
    -E, --set-end <punto de corte>: marca el �ltimo frame desde el inicio del fichero
    -E, --set-end <punto de corte>: marca la �ltima muestra desde el inicio del fichero
    -F, --fix: corrige la cabecera. usar con cuidado
    -H, --html: salida en formato html (por defecto es texto)
    -S, --shrink: recorta los silencios mayores que 'duraci�n'. usar con cuidado.
    -S, --slice <time spec>-<time spec>: hacer el corte como un segmento de tiempo
    -S, --split: separa el report por los directorios visitados
    -T, --delete-tag: borra el tag (si existe). la opci�n -D tiene que estar marcada
    -V, --version: muestra la versi�n i acaba
    -a, --all: reporta todos los ficheros, no s�lo *.mp3
    -b, --set-begin-from-eof <punto de corte>: marca el primer frame desde el final del fichero
    -b, --set-begin-from-eof <punto de corte>: marca la primera muestra desde el final del fichero
    -c, --check: comprueba el stream entero (m�s lento pero preciso)
    -d, --delete: borra el fragmento
    -d, --duration <duration>[j|s|m|b|k|M]: marca la duraci�n del fade
    -d, --duration <duraci�n>[j|s|m|b|k|M]: marca la duraci�n m�nima de silencio
    -d, --show-dirs: muestra un report de cada directorio
    -e, --set-end-from-eof <punto de corte>: marca el �ltimo frame desde el final del fichero
    -e, --set-end-from-eof <punto de corte>: marca la �ltima muestra desde el final del fichero
    -f, --force: fuerza la uni�n evitando la comprovaci�n de bit rate
    -f, --fragment [h:[m:]]s[ms] 
    -f, --show-files: muestra un report por cada fichero
    -h, --help: muestra esta ayuda i acaba
    -i, --in: s�lo fade in
    -l, --length [[h:]m:]s[.ms]: marca la duraci�n del fade.
    -l, --length [[h:]m:]s[.ms]: marca la duraci�n m�nima de silencio.
    -o, --out: s�lo fade out
    -o, --output <file>: fichero de salida donde se guarda el fragmento
    -o, --output <fichero>: envia la salida a <fichero>. de otro modo, se anexa a <fichero1>
    -q, --quiet: ning�n mensaje de salida
    -r, --recursive: escanea directorios
    -s, --show-summary: muestra un report sumario
    -s, --size <cut point>: especifica la medida del corte
    -s, --summary-only: muestra s�lo el sumario
    -t, --test: no modifica. crea i 'fade' un fichero de prueba
    -t, --threshold <lindar>: marca el valor m�ximo (en %) de las muestras silenciosas
    -v, --verbose: muestra informaci�n m�s detallada
    -v, --verbose: expl�cito
  %s: comprueba y limpia streams de mp3
  %s: detecta y recorat secuencias de silencio en ficheros wav
  %s: vuelca (i corrige) cabeceras wav
  %s: extrae y/o borra partes de un fichero mp3
  %s: extrae y/o borra partes de un fichero wav
  %s: fade in/out ficheros wav
  %s: genera reports de ficheros y directorios mp3
  %s: une ficheros mp3
  %s: une ficheros wav
  %s: muestra informaci�n de ficheros mp3
  %s: muestra informaci�n de ficheros wav
  <cut point> = <valor>[j|s|m|b|k|M]
  => %d:%02d.%03d  %u muestras  %u bytes
  bytes de basura al final  directorios    ficheros    es un fichero especial. ignorado.  no es compatible  opciones:
  mira el manual info para m�s informaci�n sobre opciones y uso
  sintaxis: %s [opci�n]... fichero
  sintaxis: %s [opci�n]... fichero...
  sintaxis: %s [opci�n]... fichero1 fichero2...
 encontrados %d ficheros err�neos
 %s - versi�n %s - creado %s
 %s: %d bytes de basura al final %s: %d de %d bytes desechables
 %s: %d:%02d de silencio en %d:%02d
 %s: %d:%02d.%03d (%d muestras) a %d:%02d:%03d (muestra %d)
 %s: '%s' no tiene suficiente espacio para una cabecera wav
 %s: '%s' es un directorio
 %s: no se puede escanear '%s'
 %s: error leyendo el estado de '%s': %s
 %s: frame inicial incompleto: %d de %d bytes
 %s: �ltimo frame (%d+1) truncado: %d de %d bytes
 %s: moviendo %d bytes %s: moviendo el tag (%d bytes) %s: la opci�n '%s' no est� reconocida o est� mal usada
 %s: saltados %d bytes %s: saltados %d bytes de basura en el inicio
 %s: saltando el tag %s: el lindar tiene que ser 0..100 (%d)
 %s: total de silencio reportado: %d:%02d.%02d (%d muestres)
 %s: truncando a %d bytes
 %u fichero%c : como m�nimo se tienen que especificar dos ficheros para unir
 : los dos fades se superponen. hay que hacerlos por separado
 : vac�o : el flag -T requiere -D. ignorando -T : ning�n fichero de entrada : sin tag : s�lo hay que especificar un fichero per cortar : menor que el silencio que se busca : vbr detectado => comprobaci�n autom�tica : el formato vbr no est� soportado [sin tag] annexando '%s' a '%s'... copiando '%s' a '%s'... creado el fichero de prueba '%s'
 cut: �ltima muestra > primera muestra error leyendo:  faded in %d muestras en '%s'
 faded out %d muestres en '%s'
 primer frame incompleto el formato est� indefinido el formato no est� reconocido:  frame fuera de rango:  error de formato de cabecera error en valor de cabecera campo inv�lido especificador de formato inv�lido:  especificaci�n de tiempo inv�lida �ltimo frame > primer frame layer no soportado incoherencia de longitud mueve %d..%d (%d muestras) a %d..%d
 ning�n '-' encontrado no se ha encontrado ninguna cabecera v�lida no es un fichero regular s�lo el layer III est� soportado las opciones (-b o -B), (-e o -E) y -s no se pueden usar juntas las opciones -B y -b no se pueden usar juntas las opciones -E y -e no se pueden usar juntas error interno de quelcom muestra fuera de rango:  escaneando '%s'... recorta %d muestras en %d
 el tag es inv�lido versi�n indefinida valor de bits/muestra no soportado n�mero de canales no soportado 