# CourseGradeDataSummary


## Program Description:

In    this   project   you    will    take  grade    and   course  data    from    two    files,    combine    it    into    a    
third  file, and    then    create    a    fourth    file    of    summary    data.  


### The Data

The   two    data   files    are    StudentData.tsv    and    HunterCourses.tsv.    Both    files    are    tab-­
delimited  (hence    the    tsv    extension    ‐    tab    separated    values).

**StudentData.tsv    (SD)**    contains    student   grade    data  
**HunterCourses.tsv    (HC)**    contains   data   about    individual    courses.  

---

**SD**  contains    6,378   lines.    Each  line    corresponds    to    a   student    taking    a   course.    Here    are    
some  example    lines:    

20424297                          1136           AFPRL                                                      10200                                                   A+                               4  
20424297                          1139            CSCI                                                                       16000                                                   W                                   ‐1    
20424297                          1142            PSYCH                                                      18000                                                   B                                         3  
20424297                          1142            PSYCH                                                      22000                                                   W                                   ­‐1    
20608974                          1082            BLOCK                                                    10000                                                   XX                               ­‐1    
20608974                          1082            CHEM                                                          102LC                                                        B+                               3.3  


The  first    column    is    the    student’s    CUNYFirst    Empl    ID,    the    second    column    is    a    code    for    
the  semester    they    took    the    course,    the    third    column    is    the    subject    code,    the    fourth    column    is    the    catalog    code    (most    times    it    is    a    number,    but    sometimes    there    are    letters    in    the    code),    the    fifth    column    is    the    letter    grade    received    and    the    sixth    column    is    the    numeric    equivalent    of    the    letter    grade.       

The  file    is  NOT  sorted    by    Empl    ID.    

Some  notes    on    the    example    lines:    
1. Student   20424297    took  four  courses    AFPRL    10200,    CSCI    16000,    PSYCH    
18000   and    PSYCH    22000.    Student  20608974    took  two    classes    BLOCK    10000    
and  CHEM    102LC.    
2. You  need    to    read    all    the    data    items,    however    you    will    only    need    to    use    the    Empl    ID,    Subject,    Catalog    Number    and    Numeric    Grade.        
3. When  asked    to    calculate    a    GPA,    numeric    grades    of    ‐1    should    be    ignored.    

---

**HC**  contains    13,875   lines.   Each   line    corresponds     to   an  active   course    offering    at    Hunter    College.    Here    are    some    example    lines:    

CSCI                                                                       12700                                                   3.0                            FSWR    

CSCI                                                                         13500                                                   3.0                            RNL    

CSCI                                                                         13600                                                   2.0                            RNL    

PSYCH                                                          22000                                                   3.0                            

The  first    column    contains    the    subject,    the    second    column    contains    the    catalog    number,    the    third    column    contains    the    number    of    contact    hours    and    the    fourth    column    contains    a    designation    code    which    indicates    whether    a   course    offering    is  a    Pathways    course,   a   liberal    arts   course,    a    non-­liberal    arts  course,    etc.    

NOTE:  Not    all    courses    in    SD    exist    in    HC.    This    happens    when    a    course    is    renumbered    
or   retired.   For    example    a    number    of    years    ago    CSCI    24500    was    changed    to    CSCI    
16000  

If    a    student    took    CSCI    24500    before    the    number    was    changed,    then    CSCI    245    
will    exist    in    the    SD  file,    but  it   won’t    be   in    the    HC    file.    

---

### The Project

Your  project    is    going    to   use    structs, linked lists,    and    one    class. Use    the    following    definitions:

    struct Class
    {
        int term;
        string subject; string catalog; string letGrade; double numGrade; Class* next ;
    };

    class ClassList
    {
        public:

        // you write all the functions for the public section

        private:
        Class* front; // pointer to the front of the ClassList
    };

    struct Student
    {
        string    ID;
        ClassList ClassesTaken; // a linked list of Class objects
    };

    struct Course
    {
        string subject; string catalog; double hours;
        string DR; // designation requirement
    };

**0)**
Load    each    file    into    a    vector.   The    StudentData    file   should    be    loaded    into    a    vector    of    Student    objects,    and    the    HunterCourses    file    should    be    loaded    into    a    vector    of    Course    objects.    You    should    only    go    through    each    file    once    ­‐    without    doing    any    calculations.


After    your    program    has    loaded    both    vectors:

**1)**
Your    program    should    first    create    a    new     file:  StudentDataPlus.tsv    which    contains    exactly    the    same    data    as    SD,    but    with    the    hours    and    the    designation    code    added    onto    each    line.    So    the    example    line    from    SD:    

20424297                          1142            PSYCH                                                      22000                                                   W                                   ‐1    

would  look    like    the    following    line    in    the    StudentDataPlus.tsv    output    file.    

20424297                          1142            PSYCH                                                      22000                                                   W                                   -­1                                      3.0                            RLA  

NOTE:  If    the   course    on   any    line    in    SD   doesn’t    exist    in    HC,    then  add:      3.0    and   RNL    to    the    end    of    the    line    in    StudentDataPlus.tsv.    Also    note    that    these    files    are    not    “toy”    example    files,    so    depending    on    your    system,    this    part    of    the    program    might    take    10’s    of    seconds    to    run.   

**2)**
 Create    a    file    called    StudentSummary.tsv    which    will    summarize    the    information    in    StudentDataPlus.tsv    for    each    student.    Each    line    should    contain    the    student’s    EmplID,    overall    GPA,    GPA    in    CSCI    courses,    and    a    percent    of   hours   spent  taking    non-­liberal    arts  courses.    For    this    assignment,    consider    the    three    codes:    RNL,    MNL,    and    GNL    as    specifying    non-­liberal    arts    courses.    All    other    designation    codes    are    for    liberal    arts    offerings.    
