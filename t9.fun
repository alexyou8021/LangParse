fun main() {
    obj = new foo()
    obj_displayx()
    obj_displayy()
    obj_displayz()
    obj_display()
    print obj_x
    print obj_y
    print obj_z

    obj1 = new foo(1)
    obj1_displayx()
    obj1_displayy()
    obj1_displayz()
    obj1_display()
    print obj1_x
    print obj1_y
    print obj1_z

    obj3 = new foo(1, 2, 3)
    obj3_displayx()
    obj3_displayy()
    obj3_displayz()
    obj3_display()
    print obj3_x
    print obj3_y
    print obj3_z
}

class foo {
    x
    y
    z

    foo() {
        x=0
        y=0
        z=0
    }

    foo(a) {
        x=a
        y=a
        z=a
    }

    foo(a, b, c) {
        x=a
        y=b
        z=c
    }

    fun displayx(){
        print x
    }

    fun displayy(){
        print y
    }

    fun displayz(){
        print z
    }

    fun display(){
        print x
        print y
        print z
    }
}

