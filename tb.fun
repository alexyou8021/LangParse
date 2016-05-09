fun main() {
    obj = new foo(1, 2, 3)
    print obj_x
    print obj_y
    print obj_z
    a = obj_display(1, 2, 3)
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
    fun display(a, b, c) {
        print a
        print b
        print c
    }
}
