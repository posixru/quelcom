��    �      �  �   �	  *     2   3     f  !   �  (   �     �     �      �  "        <  !   [  5   }     �     �     �     �     
          +     <     N  &   `  S   �  T   �  B   0  P   s  Q   �  ,     7   C  I   {  D   �  8   
  C   C  (   �  /   �  V   �  W   7  >   �  &   �  A   �  M   7  6   �  S   �  T     5   e  !   �  2   �  '   �       7   3  B   k     �  A   �  L     #   Z  %   ~  -   �  9   �  -     9   :  P   t  *   �     �  !     5   -     c  /   �  /   �     �  5   �     5     J     _     ~  $   �  '   �     �                    =  
   P  7   [     �      �  '   �     �       "   ,     O  "   o  :   �  .   �     �       "   +  +   N  0   z     �     �  .   �       1        O  "   `  6   �     �  	   �  /   �  0        A  "   I     l       (   �  '   �  !   �     �           "      <      T      k      �      �      �      �      �      !     !     .!     B!     U!     c!     ~!     �!     �!     �!  #   �!     �!     "     "     0"  =   L"  )   �"  )   �"     �"     �"     #     #     5#     D#     V#     t#  +   �#  3   �#      �#  #   $  $   3$     X$     d$  "   �$  '   �$      �$  #   �$  9   %      N%     o%     �%     �%     �%     �%     �%     �%     �%  &   &  S   +&  U   &  A   �&  O   '  Q   g'  4   �'  <   �'  J   +(  L   v(  :   �(  Q   �(  *   P)  7   {)  \   �)  ]   *  ?   n*  %   �*  B   �*  M   +  7   e+  W   �+  Y   �+  A   O,  !   �,  6   �,  +   �,     -  :   2-  C   m-     �-  A   �-  W   .  (   i.  (   �.  /   �.  :   �.  .   &/  =   U/  U   �/  2   �/     0  %   80  <   ^0  (   �0  .   �0  .   �0     "1  0   @1     q1     �1  &   �1  &   �1  '   �1  '   2     A2     [2  
   i2      t2     �2  
   �2  ;   �2     �2  "   3  +   13     ]3     y3     �3     �3  !   �3  9   �3  2   04     c4     }4  &   �4  ,   �4  1   �4     !5     55  5   R5     �5  )   �5     �5  %   �5  9    6     :6     T6  7   `6  8   �6     �6  &   �6     �6     7  /   "7  #   R7  (   v7     �7     �7     �7     �7     �7  $   8     <8     L8     h8     �8     �8     �8     �8     �8      9     9  #   +9  !   O9     q9     �9     �9  !   �9     �9  %   �9     :      ':  =   H:  +   �:  +   �:     �:     �:     ;     ;     8;     L;      ^;     ;  �   �   )           H   /   ^   w       {   �   �   5       �   �   �   �   T           �   C   	   %   $   ;   `   L   ?   r       |   <   v   y   k       ]   �   �   9       ~      �       R      z      P   J   t           U   O                           #           �   @       S      -   2           G              q   &                      s   E      �      �       I           u   V          f   �   �          _       Q              X   �   M   i           �   B   g   n      
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
 tag is invalid undefined version unsupported bits/sample value unsupported channel value 		 el camp bytes/mostra ha de ser 1, 2 o 4
 		el camp channels ha de ser 1 (mono) o 2 (stereo)
 		el camp data ha de ser 'data'
 		el camp data length ha de ser %d
 		no s� quin valor s'ha de posar...
 		corregit
 		el camp fmt ha de ser 'fmt '
 		el camp fmt length ha de ser %d
 		el camp format ha de ser 1 (pcm tag)
 		el camp riff ha de ser 'RIFF'
 		el camp riff length ha de ser %d
 		la velocitat de mostreig ha d'estar entre 8000 i 48000
 		el camp wave ha de ser 'WAVE'
 	bits/mostra: %d
 	bytes/mostra: %d
 	bytes/segon: %d
 	canals: %d
 	data length: %d
 	fmt length: %d
 	riff length: %d
 	velocitat de mostreig: %d
    -A, --show-all: implica -d, -f, -s
    -B, --set-begin <punt de tall>: marca el primer frame des de l'inici del fitxer
    -B, --set-begin <punt de tall>: marca la primera mostra des de l'inici del fitxer
    -D, --delete: esborra frames inv�lids i brossa. usar amb cura
    -E, --set-end <punt de tall>: marca l'�ltim frame des de l'inici del fitxer
    -E, --set-end <punt de tall>: marca l'�ltima mostra des de l'inici del fitxer
    -F, --fix: corregeix la cap�alera. usar amb cura
    -H, --html: sortida en format html (per defecte �s text)
    -S, --shrink: retalla els silencis majors que 'durada'. usar amb cura.
    -S, --slice <time spec>-<time spec>: fer el tall com un segment de temps
    -S, --split: trenca el report pels directoris visitats
    -T, --delete-tag: esborra el tag (si existeix). l'opci� -D ha d'estar marcada
    -V, --version: mostra la versi� i surt
    -a, --all: reporta tots els fitxers, no nom�s *.mp3
    -b, --set-begin-from-eof <punt de tall>: marca el primer frame des de l'inici del fitxer
    -b, --set-begin-from-eof <punt de tall>: marca la primera mostra des del final del fitxer
    -c, --check: comprova l'steam sencer (m�s lent per� prec�s)
    -d, --delete: esborra el fragment
    -d, --duration <durada>[j|s|m|b|k|M]: marca la durada del fade
    -d, --duration <durada>[j|s|m|b|k|M]: marca la durada m�nima d'un silenci
    -d, --show-dirs: mostra un report de cada directori
    -e, --set-end-from-eof <punt de tall>: marca l'�ltim frame des del final del fitxer
    -e, --set-end-from-eof <punt de tall>: marca l'�ltima mostra des del final del fitxer
    -f, --force: for�a la uni� evitant la comprovaci� de bit rate
    -f, --fragment [h:[m:]]s[ms] 
    -f, --show-files: mostra un report per cada fitxer
    -h, --help: mostra aquesta ajuda i surt
    -i, --in: nom�s fade in
    -l, --length [[h:]m:]s[.ms]: marca la durada del fade.
    -l, --length [[h:]m:]s[.ms]: marca la durada m�nima de silenci.
    -o, --out: nom�s fade out
    -o, --output <file>: fitxer de sortida on es desa el fragment
    -o, --output <fitxer>: envia la sortida a <fitxer>. altrament, s'annexa a <fitxer1>
    -q, --quiet: cap missatge de sortida
    -r, --recursive: escaneja directoris
    -s, --show-summary: mostra un report sumari
    -s, --size <punt de tall>: especifica la mida del tall
    -s, --summary-only: mostra nom�s el sumari
    -t, --test: no modifica. crea i 'fade' un fitxer de prova
    -t, --threshold <llindar>: marca el valor m�xim (en %) de les mostres silencioses
    -v, --verbose: mostra informaci� m�s detallada
    -v, --verbose: expl�cit
  %s: comprova i neteja streams d'mp3
  %s: detecta i retalla seq��ncies de silenci en fitxers wav
  %s: aboca (i corregeix) cap�aleres wav
  %s: extreu i/o esborra parts d'un fitxer mp3
  %s: extreu i/o esborra parts d'un fitxer wav
  %s: fade in/out fitxers wav
  %s: genera reports de fitxers i directoris mp3
  %s: ajunta fitxers mp3
  %s: ajunta fitxers wav
  %s: mostra informaci� de fitxers mp3
  %s: mostra informaci� de fitxers wav
  <punt de tall> = <valor>[j|s|m|b|k|M]
  => %d:%02d.%03d  %u mostres  %u bytes
  bytes de brossa al final  directoris    fitxers    �s un fitxer especial. ignorat.  no �s compatible  opcions:
  mira el manual info per m�s informaci� sobre opcions i �s
  sintaxi: %s [opci�]... fitxer
  sintaxi: %s [opci�]... fitxer...
  sintaxi: %s [opci�]... fitxer1 fitxer2...
 trobats %d fitxers erronis
 %s - versi� %s - creat %s
 %s: %d bytes de brossa al final %s: %d de %d bytes rebutjables
 %s: %d:%02d de silenci a %d:%02d
 %s: %d:%02d.%03d (%d mostres) a %d:%02d:%03d (mostra %d)
 %s: '%s' no t� prou espai per a una cap�alera wav
 %s: '%s' �s un directori
 %s: no es pot escanejar '%s'
 %s: error llegint l'estat de '%s': %s
 %s: frame inicial incomplet: %d de %d bytes
 %s: darrer frame (%d+1) escap�at: %d de %d bytes
 %s: movent %d bytes %s: movent el tag (%d bytes) %s: l'opci� '%s' no est� reconeguda o est� mal usada
 %s: saltats %d bytes %s: saltats %d bytes de brossa a l'inici
 %s: saltant el tag %s: el llindar ha de ser 0..100 (%d)
 %s: total de silenci reportat: %d:%02d.%02d (%d mostres)
 %s: escap�ant a %d bytes
 %u fitxer%c : com a m�nim s'han d'especificar dos fitxers per unir
 : els dos fades se sobreposen. s'han de fer per separat
 : buit : el flag -T requereix -D. ignorant -T : cap fitxer d'entrada : sense tag : nom�s s'ha d'especificar un fitxer per tallar : menor que el silenci que es busca : detectat vbr => comprovaci� autom�tica : el format vbr no se suporta [sense tag] annexant '%s' a '%s'... copiant '%s' a '%s'... creat el fitxer de prova '%s'
 cut: darrera mostra > primera mostra error llegint:  faded in %d mostres a '%s'
 faded out %d mostres a '%s'
 primer frame incomplet el format �s indefinit el format no es reconeix:  frame fora de rang:  error de format de cap�alera error en valor de cap�alera camp inv�l.lid especificador de format inv�l.lid:  especificaci� de temps inv�l.lida darrer frame > primer frame layer no suportat incoher�ncia de llargada mou %d..%d (%d mostres) a %d..%d
 cap '-' trobat no s'ha trobat cap cap�alera v�l.lida no �s un fitxer regular nom�s el layer III est� suportat les opcions (-b o -B), (-e o -E) i -s no es poden usar alhora les opcions -B i -b no es poden usar alhora les opcions -E i -e no es poden usar alhora error intern de quelcom mostra fora de rang:  escanejant '%s'... retalla %d mostres a %d
 el tag �s inv�l.lid versi� indefinida valor de bits/mostra no suportat nombre de canals no suportat 