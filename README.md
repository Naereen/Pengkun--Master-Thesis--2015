## Implementation And Experimentation of *Coherent Inference in Game Tree*
> Implementation in C++, forked from [Charles-Lau-/thesis](https://github.com/Charles-Lau-/thesis).

### Usage of this program
You can either use the [Makefile](./code/Makefile) or the [run](./code/run) script, or compile manually:

```bash
# Compile the program
g++ -std=c++11 -o playGame *.cpp algorithm/*.cpp
# Run it (and keep its output)
./playGame | tee -a ./playGame.log
```

You can have a look to the (long) [playGame.log](./code/playGame.log) file for an example of its output.

### :bug: Limitation?
The implementation is generic enough to be able to play to higher dimension [Tic-Tac-Toe](https://en.wikipedia.org/wiki/Tic-Tac-Toe), not only the usual 3x3.

### Example with a 4x4 Tic-Tac-Toe
When the player **X** has to move, it examines a few possible moves, and compute the Gaussian value function learned for each position.
For instance, image that **X** has to play on this board state:
```
O O X O
_ _ _ X
O _ X X
_ X X O
```

Then it will explore all the possible moves (6 in our example):
```
V value: Gaussian N(-0.224098, 0.862775)...
O O X O
_ _ _ X
O _ X X
_ X X O

... 2 other moves

V value: Gaussian N(0.939144, 0.521763)...  <-- The best one!
O O X O
_ _ X _
O _ X X
_ X X O


V value: Gaussian N(-1.40412, 0.9321)...
O O X O
_ _ _ _
O X X X
_ X X O


V value: Gaussian N(-0.658714, 0.932628)...
O O X O
_ _ _ _
O _ X X
X X X O
```

Finally, the Coherent Inference algorithm will sample from each of these Gaussian distribution (associated to each children of the current node).
It selects the move corresponding to the highest sample (usuall, from the more optimistic Gaussian value function V).

In the example showed above, ``N(0.93, 0.52)`` has a mean value of **0.93** (really higher than any other), so with high probability the sampling will select this action.
Here it is the (only) one which directly brings **X** to a winning position:

```
O O X O
_ _ X _
O _ X X
_ X X O
```

This was only a very rough explanation of the algorithm.
For all the details, please refer to the initial paper, or [the slides or report for my project](https://bitbucket.org/lbesson/mva15-project-graph-reinforcement-learning/downloads/).

At the end, the *coherent Gaussian inference player* (**X**) **wins 72% of the time**, which for 4x4 Tic-Tac-Toe is an extremely satisfactory result! :tada:

----

### Overview of layout of the program
#### ``Board.main()`` (in the [code](./code/) folder)
Entrance of the program:

- In this function, two computer players are initialized; and the ``Board`` is initialized.
- Then a variable rounds is set, which means how many rounds are played between these two players.


#### ``ComputerPlayer`` (in the [code](./code/) folder)
Model a ComputerPlayer:

- In this module, there are two functions, which are ``randomPlay`` and ``algorithmPlay`` respectively. These two functions will change the current board state. And they both return an identifier, which can let ``Board.main()`` determine whether the game should be continued or the game is winned by a player.
- In ``randomPlay``, the player just plays the game randomly, while the player plays according to the algorithm in ``algorithmPlay``.
- In ``algorithmPlay``, just three things are done: first, set up parameters like number of descents, the root of descent tree; second, call ``Ep.descent`` to do descent whose parameter the the root; third, determine which move is the best one.


#### ``Ep`` (Expectation-Propagation) (in the [algorithm](./code/algorithm/) folder)
Core of the expectation propagation algorithm:

- Basically, in this module, the algorithm is implemented and all related message passing methods are implemented.
- The method descent is the backbone and other message passing methods serve for it.
- Additionally, all calculation related to Gaussian distribution are carried out in ``Distribution`` module.


#### ``Distribution`` (in the [algorithm](./code/algorithm/) folder)
- All dirty works of calculation about Gaussian distributions and correlated coefficient.
- In this module, all messy distribution and correlated coefficient calcualtions are implemented.
- Notice should be paid tothe Min/Max calculation of multiple dependent Gaussian distributions.

#### ``Node`` (in the [algorithm](./code/algorithm/) folder)
Support build-up of ``Game`` tree:

- It is an entity  module, which supports implementation of ``Game`` node in a ``Game`` tree.
- In every node, it has both pointer to its parent and its children.
- Also, binded values and distributions are its properties.
- Notice: the ``getChild()`` function is varied if the ``Board`` is changed. For different game board, the strategy to get valid children nodes of a specific node is varied

#### Other files?
> - The rest of the project are programs either easier to understand or less useful.
> - [See this other project for a more simple Reinforcement Learning agent for 3x3 Tic-Tac-Toe](http://naereen.github.io/Wesley-Tansey-RL-TicTacToe/).

----

### Authors
- Forked and cleaned up by [Lilian Besson (Naereen)](https://github.com/Naereen), 28/12/2015, for [my MVA master project for the "Reinforcement Learning" course](https://bitbucket.org/lbesson/mva15-project-graph-reinforcement-learning/).
- Created by [Pengkun Liu](https://github.com/Charles-Lau-/), April - June 2015,

### :scroll: License ? [![GitHub license](https://img.shields.io/github/license/Naereen/Pengkun--Master-Thesis--2015.svg)](https://github.com/Naereen/Pengkun--Master-Thesis--2015/blob/master/LICENSE)
[MIT Licensed](https://lbesson.mit-license.org/) (file [LICENSE.txt](LICENSE.txt)).

[![Maintenance](https://img.shields.io/badge/Maintained%3F-no-red.svg)](https://GitHub.com/Naereen/Pengkun--Master-Thesis--2015/graphs/commit-activity)
[![Ask Me Anything !](https://img.shields.io/badge/Ask%20me-anything-1abc9c.svg)](https://GitHub.com/Naereen/ama)
[![Analytics](https://ga-beacon.appspot.com/UA-38514290-17/github.com/Naereen/Pengkun--Master-Thesis--2015/README.md?pixel)](https://GitHub.com/Naereen/Pengkun--Master-Thesis--2015/)

[![ForTheBadge uses-badges](http://ForTheBadge.com/images/badges/uses-badges.svg)](http://ForTheBadge.com)
[![ForTheBadge uses-git](http://ForTheBadge.com/images/badges/uses-git.svg)](https://GitHub.com/)

[![ForTheBadge built-with-science](http://ForTheBadge.com/images/badges/built-with-science.svg)](https://GitHub.com/Naereen/)
