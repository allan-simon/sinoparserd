sinoparserd
===========

A service to convert chinese languages (mandarin. cantonese. shanghainese..) into their transliterated form. to segment them etc.

The license only apply to the source code
The data files are provided only as example



### Requirement ###

  * libexpat
  * libevent

### Install ###

If you never used `cmake` have a look at [this tutorial](http://web.cs.swarthmore.edu/~adanner/tips/cmake.php).
```bash
git clone git://github.com/allan-simon/sinoparserd
sudo apt-get install libexpat1{,-dev} libevent-{2.0-5,dev} cmake
cd sinoparserd/ && cmake ./ && make
```
This should output something like:
  > ![compilation screenshot](compilation-screenshot.png)

### Usage ###

    ./sinoparserd -c  doc/cantonese.xml -m doc/mandarin.xml -p PORT

That will launch an HTTP server listening on port `PORT`. If the option `-p` is missing it will listen on port `8080`

it provides the following API call, that will return a XML answer

  * [/pinyin?str=\*](http://localhost:8080/pinyin?str=\*)
  * [/jyutping?str=\*](http://localhost:8080/jyutping?str=\*)
  * [/change\_script?str=\*](http://localhost:8080/change\_script?str=\*)
  * [/trad?str=\*](http://localhost:8080/trad?str=\*)
  * [/simp?str=\*](http://localhost:8080/simp?str=\*)
  * [\_script?str=\*](http://localhost:8080/guess\_script?str=\*)
  * [/all?str=\*](http://localhost:8080/all?str=\*)

## License

All the source code is licensed under GPLv3, the xml files are under their own license, it's a "open one" but i need to check which one, certainly CC-BY-SA
so for the moment I would recommend people to use their own data files for "public usage" and use the provided xml only for "test" purpose.

