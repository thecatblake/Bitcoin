package field

import (
	"math/big"
	"testing"
)

func TestElementEquality(t *testing.T) {
	a := newElement(big.NewInt(7), big.NewInt(13))
	b := newElement(big.NewInt(6), big.NewInt(13))
	c := newElement(big.NewInt(7), big.NewInt(17))
	d := newElement(big.NewInt(6), big.NewInt(17))
	e := newElement(big.NewInt(7), big.NewInt(13))

	if equal(a, b) {
		t.Fatalf("a != b")
	}

	if equal(a, c) {
		t.Fatalf("a != c")
	}

	if equal(a, d) {
		t.Fatal(a, d)
	}

	if !equal(a, e) {
		t.Fatal(a, e)
	}
}

func TestAddition(t *testing.T) {
	a := Element{big.NewInt(7), big.NewInt(13)}
	b := Element{big.NewInt(6), big.NewInt(13)}
	y1 := Element{big.NewInt(0), big.NewInt(13)}
	y2 := Element{big.NewInt(1), big.NewInt(13)}

	if !equal(Add(a, b), y1) {
		t.Fatal("Addition not working correctly")
	}

	if !equal(Sub(a, b), y2) {
		t.Fatal("Subtraction not working correctly")
	}
}

func TestMultiplication(t *testing.T) {
	a := Element{big.NewInt(3), big.NewInt(31)}
	b := Element{big.NewInt(24), big.NewInt(31)}
	y1 := Element{big.NewInt(10), big.NewInt(31)}
	y2 := Element{big.NewInt(4), big.NewInt(31)}

	if !equal(Mul(a, b), y1) {
		t.Fatal("Multiplication not working correctly")
	}

	if !equal(Div(a, b), y2) {
		t.Fatal("Division not working correctly")
	}
}
