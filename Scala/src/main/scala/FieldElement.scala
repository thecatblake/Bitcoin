import scala.annotation.targetName

class FieldElement(val num: BigInt, val prime: BigInt):
  @targetName("finite_field_addition")
  def +(other: FieldElement): FieldElement =
    FieldElement((num + other.num) % prime, prime)

  @targetName("finite_field_subtraction")
  def -(other: FieldElement): FieldElement =
    var n = (num - other.num) % prime
    if n < 0 then
      n += prime
    FieldElement(n, prime)

  @targetName("finite_field_multiplication")
  def *(other: FieldElement): FieldElement =
    FieldElement((num * other.num) % prime, prime)

  @targetName("finite_field_exponent")
  def **(exponent: BigInt): FieldElement =
    val n = exponent % (prime - 1)
    FieldElement(num.modPow(n, prime), prime)

  @targetName("finite_field_division")
  def /(other: FieldElement): FieldElement =
    this * (other ** (prime - 2))

  override def toString: String =
    s"FieldElement($num, $prime)"

  @targetName("Ffinite_field_equality")
  def ==(obj: FieldElement): Boolean =
    num == obj.num && prime == obj.prime