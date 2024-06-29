
This repository contains a single header math library. The library includes a number of common mathematical structures,
constants and operations. Requires C++20.<br/><br/>
Everything is templated for easy adjustment to requirements. 
All structures are supplied with casting operators. 
All structures support formatted stream output.<br/><br/>
Currently implemented:<br/>
- Vectors (general with templated size, 2D, 3D, 4D)
	- Arithmetic operations with scalars and vectors (preserve precision)
	- Calculation of magnitude, average, dot product, distance
	- Rotation
- Segment (2D)
	- Arithmetic operations
	- Calculation of length, center, surface vector
- Matrix (2D)
	- Matrix-vector multiplication
- Free functions
	- Angle conversion (radians to degrees and back)
	- Average
	- Distance (between vectors, between vector and segment)
