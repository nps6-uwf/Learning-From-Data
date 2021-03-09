# Learning-From-Data
Exercises, problems, and other content from Learning From Data - Abu Mostafa

## Exercise 1.10) See C++ code for full solution.
<img src="https://github.com/nps6-uwf/Learning-From-Data/blob/main/assets/histo.PNG?raw=true"></img>

## Exercise 1.12)

A friend comes to you with a learning problem. She says the target function is completely unknown, but she has 4, 000 data points. She is 
willing to pay you to solve her problem and produce for her a <i>g</i> which 
approximates <i>f</i>. What is the best that you can promise her among the 
following: 

<ol>
  <li>After learning you will provide her with a <i>g</i> that you will guarantee 
approximates well out of sample. </li>  
  <li>After learning you will provide her with a <i>g</i>, and with high probability 
the <i>g</i> which you produce will approximate well out of sample. </li>
  <li>
  One of two things will happen: 

  <ol>
    <li>
    You will produce a hypothesis <i>g</i>; 
    </li>
    <li>
    You will declare that you failed. 
    </li>
  </ol>
  If you do return a hypothesis <i>g</i>, then with high probability  the <i>g</i> which 
  you produce will approximate well out of sample. 
  </li>
</ol>

    solution:
    
    You cannot guarantee your friend that g will approximate well out of sample.  The problem statement does not explicity state 
    that a pattern exists in the data and does not discuss the set of possible hypotheses, and finally Hoeffding is a probabilistic 
    bound so we can only gurantee relative to some probability.  The correct choice is (3)

