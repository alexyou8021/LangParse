fun main() {
        obj = new Class(1,2,3)
        print obj.sum()
}

class Class {
	x
	y
	z

        Class() {
                x=0
        }
        Class(a) {
                x=a
                y=a
                z=a
        }
        Class(a, b, c) {
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
