fun main() {
    obj = new foo()
    a = obj_displayx()
    a = obj_displayy()
    a = obj_displayz()
    a = obj_display()
    print obj_x
    print obj_y
    print obj_z

    obj1 = new foo(1)
    b = obj1_displayx()
    b = obj1_displayy()
    b = obj1_displayz()
    b = obj1_display()
    print obj1_x
    print obj1_y
    print obj1_z

    obj3 = new foo(1, 2, 3)
    c = obj3_displayx()
    c = obj3_displayy()
    c = obj3_displayz()
    c = obj3_display()
    print obj3_x
    print obj3_y
    print obj3_z
}

class foo {
    x=0
    y=0
    z=0

    foo() {
        x=1
        y=1
        z=1
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
}
