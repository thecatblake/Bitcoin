import org.scalatest.funsuite.AnyFunSuite

class FieldElementTest extends AnyFunSuite{
  test("FieldElement.addition") {
    val a = FieldElement(7, 13)
    val b = FieldElement(6, 13)

    assert(a + b == FieldElement(0, 13))
    assert(a - b == FieldElement(1, 13))
    assert(b - a == FieldElement(12, 13))
  }

  test("FieldElement multiplication") {
    val a = FieldElement(3, 31)
    val b = FieldElement(24, 31)

    assert(a * b == FieldElement(10, 31))
    assert(a / b == FieldElement(4, 31))
  }
}
