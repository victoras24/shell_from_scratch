Same reasons as the websockets from scratch bring me here. This time I would like to take a different route and instead of navigating through the RFCs, I'm following the course from codecrafters, which my friend has suggested to start. The goals of this project are the same as the websockets from scratch, the only difference is that we use the C language. I don't have any working experience with C, I used it for a month, trying to create a game engine (didn't go very well). 

Starting the course, you have to go through some easy steps to get to the actual coding stuff. At the beginning I'm trying to understand how to start, as I'm used to the front end React apps. Here, the thought process is different. All of us have heard of pointers and wonder what it is? What is the purpose of pointers? Why is the no such thing in javascript? All of that I'm planning to understand and learn in this journey. Don't get confused, I'm planning to learn as much as I can about C, it's just that pointers are a well-known part of the language, to all the developers. 

Going through the 3rd to 6th lesson, I was just passing the tests. I called my friend, to have a look and give me advises on how to move forward. Note that he doesn't have experience in C, but he is the best typescript developer that I know. He is a proper engineer, taking each step to the maximum and extending every concept, so I'm trying to take as much knowledge as I can. I saw his work at the same stage I'm at, the 6th lesson that I'm in the 6th lesson and it's like night and day. I have a single main file, with helper functions, arrays and a bloated main function inside. Looking at his code, surprisingly he structured the codebase completely differently from me. Even though he doesn't have a clue about C and low level programming in general, he was able to structure his codebase like a pro. He created header files, created structs and then type aliases (I still don't know exactly what they are), enums, unions, switch statements. I felt like he knows exactly what he is doing. I understood then, an engineer is an engineer. Whatever language or environment an engineer is in, even an agentic one, he knows how to handle it. 

So from now on I will try to dig into each problem and try to find better solutions. Try to extend the code and think for future cases. Understand each concept deeply instead of skipping through. This read me file will be a place where I can come back in 3 years and compare the progress. Hopefully I will not be in the same position xD.

Note that what follow may have mistakes, please refer to the official documentation if you would like to get correct information. I feel like typing and analyzing each new concept, helps me to understand and find better solutions. 

As you can see in the commits [Commit 1](https://github.com/victoras24/shell_from_scratch/commit/cd25e1f71577297321f4f148cdd6fc52df16d7b2), [Commit 2](https://github.com/victoras24/shell_from_scratch/commit/f4c2269be5afc5169b2b962723ee0e0f0fdb6b89), [Commit 3](https://github.com/victoras24/shell_from_scratch/commit/caf40b90bb0093a989ab3fa7c12cd3de01a733ab) I started refactoring the code. Tried to create an enum using "X Macros". It's an ancient technique, as Tsoding said, where you can generate code in the preprocessor stage, before the compiler runs. It's useful when you want to have one source of truth and there is zero runtime cost, it's all text done before compiling. Yep I know that it's overkill for a small list, I'm just trying to learn. Let me show you how is done.
Let's say you have a list of fruits and you would like to create an enum. There a nice surprise you get with the enum, that I did not think of it. First you will have to define the items. The items are calls to a macro that does not exists yet.
```c
#define LIST_OF_FRUITS            \
  GEN(apple, "apple")             \
  GEN(banana, "banana")           \
  GEN(pear, "pear")
```
Each backslash continues the macro to the next line. Next step is to define the macro.
```c
#define GEN(id, str) id
```
What you can do is add a prefix to avoid name clashes. Don't forget the comma, otherwise items will run together.
```c
#define GEN(id, str) fruit_##id,
```
Now let's expand the list
```c
enum fruit {LIST_OF_FRUITS fruit_count};
```
Don't forget to undefine the macro. It's a good practice to avoid redefining the macro.
```c
#undef GEN
```
And last you can create the list in the c file.
```c
#define GEN(id, str) str,
const char *fruit_names[] = { LIST_OF_FRUITS };
#undef GEN
```
You can check your work at any stage, looking at what the preprocessor produced.
```sh
gcc -E file.c
```
