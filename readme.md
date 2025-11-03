the implementation works as followed, 
the way the AI works: 
    - it recursivaly looks for an end state of the game 
    - when it detemines an end state it updates the value and we are returning it to the recursive version above 
    - since it doesn't always reach an end state the EvaluateStateForPlayer funcion determines how many pieces it gets in a row and returns that the amount times a multiplier 
    - the player switches through each recurive level and in each level the AI assumes the best move for that player this version automatically plays with AI over being able to choose to play with one.