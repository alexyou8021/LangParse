fun main() {
    obj1 = new object()
    a = obj1_display()
    obj2 = new object(1)
    b = obj2_display()
    obj3 = new object(1,2,3)
    c = obj3_display()
}

class object {
    x = 0
    y = 0
    z = 0

    object() {
        x=0
        y=0
        z=0
    }

    object(a) {
        x=a
        y=a
        z=a
    }
    object(a, b, c) {
        x=a
        y=b
        z=c
    }

    fun display() {
        print x
        print y
        print z
    }
}
