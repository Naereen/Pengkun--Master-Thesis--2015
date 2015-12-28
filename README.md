## Implementation And Experimentation of *Coherent Inference in Game Tree*
> Implementation in C++, forked from [Charles-Lau-/thesis](https://github.com/Charles-Lau-/thesis).

### Usage of this program
You can either use the [Makefile](code/Makefile) or the [run](code/run) script, or compile manually:

```bash
# Compile the program
g++ -std=c++11 -o playGame *.cpp algorithm/*.cpp
# Run it (and keep its output)
./playGame | tee -a ./playGame.log
```

You can have a look to the (long) [playGame.log](code/playGame.log) file for an example of its output.

----

### Overview of layout of the program
#### ``Board.main()``
Entrance of the program:

- In this function, two computer players are initialized;and the Board is initialized.
- Then a variable rounds is set, which means how many rounds are played between these two players.


#### ``ComputerPlayer``
Model a ComputerPlayer:

- In this module, there are two functions, which are ``randomPlay`` and ``algorithmPlay`` respectively. These two functions will change the current board state. And they both return an identifier, which can let ``Board.main()`` determine whether the game should be continued or the game is winned by a player.
- In ``randomPlay``, the player just plays the game randomly, while the player plays according to the algorithm in algorithmPlay.
- In ``algorithmPlay``, just three things are done: first, set up parameters like number of descents, the root of descent tree; second, call ``Ep.descent`` to do descent whose parameter the the root; third, determine which move is the best one.


#### ``Ep`` (Expectation-Propagation)
Core of the expectation propagation algorithm:

- Basically, in this module, the algorithm is implemented and all related message passing methods are implemented.
- The method descent is the backbone and other message passing methods serve for it.
- Additionally, all calculation related to Gaussian distribution are carried out in ``Distribution`` module.


#### ``Distribution``
- All dirty works of calculation about Gaussian distributions and correlated coefficient.
- In this module, all messy distribution and correlated coefficient calcualtions are implemented.
- Notice should be paid tothe Min/Max calculation of multiple dependent Gaussian distributions.

#### ``Node``
Support build-up of ``Game`` tree:

- It is an entity  module, which supports implementation of ``Game`` node in a ``Game`` tree.
- In every node, it has both pointer to its parent and its children.
- Also, binded values and distributions are its properties.
- Notice: the ``getChild()`` function is varied if the ``Board`` is changed. For different game board, the strategy to get valid children nodes of a specific node is varied

----

### License and authors
- Forked and cleaned up by [Lilian Besson (Naereen)](https://github.com/Naereen), 28/12/2015,
- Created by [Pengkun Liu](https://github.com/Charles-Lau-/), June 2015,
- Code released under the [MIT license](http://lbesson.mit-license.org).
