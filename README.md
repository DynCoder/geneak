# geneak

Branch|[![travis_ci](doc/travis-ci.png)](https://travis-ci.org/Joshua260403/geneak)
---|---
Master|[![Build Status](https://travis-ci.org/Joshua260403/geneak.svg?branch=master)](https://travis-ci.org/Joshua260403/geneak)
Backup|[![Build Status](https://travis-ci.org/Joshua260403/geneak.svg?branch=backup)](https://travis-ci.org/Joshua260403/geneak)
Release|[![Build Status](https://travis-ci.org/Joshua260403/geneak.svg?branch=release)](https://travis-ci.org/Joshua260403/geneak)

## License
All code in this project is licensed under the GPLv3 license.
Refer to [LICENSE](LICENSE) for the full license.\
___(Especially when you're from the other group working on this project)___

## the name
The name `geneak` comes from the basque word for genes.\
___I am not going to explain why basque.___

## the idea
The idea is to create a piece of software that can be used to draw filogenies in a matter of minutes. This is to solve the problem of biologists always having to draw filogenies on a piece of paper and it ending up very messy.

## Features
#### The software should have at least the following features:
- Draw a tree in the software
- Generate a formula from input (see example below)
- Save the newick to a `.gnk` file
- Open `.gnk` files

##### Example:
```
((A,B),C,(D,(E,F)));

     -- A
  ---|
  |  -- B
  |
--|-- C
  |
  |  -- D
  ---|
     |  -- E
     ---|
        -- F
```

## References
* [license](LICENSE)
* [team](doc/team.md)
