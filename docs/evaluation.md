## Evaluation

### 1. Runtime Performance
1. Test exmaple programs with high load
    1. Using raw PMDK
    2. Using pm-wrapper with PMDK backend
2. Evaluate factor or absolute time between both programs
3. Reasons for slower performance
4. Ways to improve permformance

### 2. Evalution of usability
1. Qualitative evaluation of porting an existing c program to pm-warpper
    1. One good example        
    2. One bad example
    3. Metrics
        1. How readable is the code
            1. Compared to PMDK
            2. Compared to the original C program
        2. How many attributes have to be used
        3. How many refactorings have to be done
        4. How many data structure changes have to be done
        5. How helpful are error/warning messages
    4. Areas to improve and limitations

### 3. Evaluation of abstraction
1. Qualitative evaluation about how much work has to be done to implement a backend
2. Metrics:
    1. Additionally needed data structures or logics inside the backend
    2. Can the backend cover usecases which are not yet implemented

### 4. Evaluation of complexity of integration into existing pipelines
1. Cmake