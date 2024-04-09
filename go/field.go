package field

import (
	"math/big"
)

type Element struct {
	num   *big.Int
	prime *big.Int
}

func newElement(n *big.Int, p *big.Int) Element {
	return Element{n, p}
}

func Add(e1 Element, e2 Element) Element {
	num := new(big.Int)
	num.Add(e1.num, e2.num)
	num.Mod(num, e1.prime)
	return Element{num, e1.prime}
}

func Sub(e1 Element, e2 Element) Element {
	num := new(big.Int)
	num.Sub(e1.num, e2.num)
	num.Mod(num, e1.prime)
	return Element{num, e1.prime}
}

func Mul(e1 Element, e2 Element) Element {
	num := new(big.Int)
	num.Mul(e1.num, e2.num)
	num.Mod(num, e1.prime)
	return Element{num, e1.prime}
}

func Pow(e Element, n big.Int) Element {
	num := new(big.Int)
	num.Exp(e.num, &n, e.prime)
	return Element{num, e.prime}
}

func Div(e1 Element, e2 Element) Element {
	inv := new(big.Int)
	inv.ModInverse(e2.num, e1.prime)
	num := new(big.Int)
	num.Mul(e1.num, inv)
	num.Mod(num, e1.prime)
	return Element{num, e1.prime}
}

func equal(e1 Element, e2 Element) bool {
	return e1.num.Cmp(e2.num) == 0 && e1.prime.Cmp(e2.prime) == 0
}

func (e *Element) String() string {
	return "FieldElement(" + e.num.String() + "," + e.prime.String() + ")"
}
