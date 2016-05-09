fun main() {
    obj = new object(1,2,3)
    a = obj_display()
    print obj_x
    print obj_sum()
    print obj_x+obj_y+obj_z
}

class object {
    x = 0
    y = 0
    z = 0

    object() {
        x=0
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

    fun sum() {
        return x+y+z
    }
}
