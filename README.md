# geneak
[:calendar: may-june 2019](doc/planner.md)

Branch|[![travis_ci](doc/travis-ci.png)](https://travis-ci.org/Joshua260403/geneak)
---|---
Master|[![Build Status](https://travis-ci.org/Joshua260403/geneak.svg?branch=master)](https://travis-ci.org/Joshua260403/geneak)

## License
All code in this project is licensed under the GPLv3 license.
Refer to [LICENSE](LICENSE) for the full license.\
___(Especially when you're from the other group working on this project)___

## the name
The name `geneak` comes from the basque word for genes.\
___I am not going to explain why basque.___

## the idea
The idea is to create a piece of software that can be used to create filogenetic trees in a matter of minutes. This is to solve the problem of biologists always having to draw the trees on a piece of paper and it ending up very messy.

## Features
#### The software should have at least the following features:
- Draw a tree in the software
- Draw a tree following input (see below for example)
- Give a formula from input (the example below in reverse)
- Save the tree drawn into (for example) a `.tree` file

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

#### Extra features:
- Extinct species / Timeline

## References
* [license](LICENSE)
* [team](doc/team.md)
