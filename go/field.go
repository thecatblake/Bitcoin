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

func add(e1 Element, e2 Element) Element {
	num := new(big.Int)
	num.Add(e1.num, e2.num)
	num.Mod(num, e1.prime)
	return Element{num, e1.prime}
}

func sub(e1 Element, e2 Element) Element {
	num := new(big.Int)
	num.Sub(e1.num, e2.num)
	num.Mod(num, e1.prime)
	return Element{num, e1.prime}
}

func equal(e1 Element, e2 Element) bool {
	return e1.num.Cmp(e2.num) == 0 && e1.prime.Cmp(e2.prime) == 0
}

func (e *Element) String() string {
	return "FieldElement(" + e.num.String() + "," + e.prime.String() + ")"
}
