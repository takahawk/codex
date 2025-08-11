package main
import (
	"fmt"
	"os"

	"strconv"
	"time"
)

func main() {
	n, _ := strconv.Atoi(os.Args[1])
	
	var a []int
	for i := 0; i < n; i++ {
		a = append(a, n - i)
	}

	start := time.Now()
	for i:=0; i < len(a)-1; i++ {
		for j:=0; j < len(a)-i-1; j++ {
			 if (a[j] > a[j+1]) {
					a[j], a[j+1] = a[j+1], a[j]
				}
		}
	}

	duration := time.Since(start)

	fmt.Printf("%.6f\n", duration.Seconds())
}
