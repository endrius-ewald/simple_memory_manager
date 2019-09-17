# Simple Memory Manager
It's a simple memory manager made in C. It supports a few set of create, read and update functions in a small set of data types. 
Related to PBN class in 17A. 

## 1 - Suported instructions
### Create
CREATE_CHAR, CREATE_INT, CREATE_FLOAT, CREATE_VET_CHAR, CREATE_VET_INT, CREATE_MAT_FLOAT
* CREATE_< type> <var_name>
* CREATE_VET_< type> < var_name> < var_size>
* CREATE_MAT_< type> < var_name> < line_size> < col_size>

### Read
  PRINT, dump 
* PRINT < var_name>
* PRINT < var_name> < position>
* PRINT < var_name> < line> < collumn>
* dump
  
### Update
  SET
* SET < var_name> < value>
* SET < var_name> < position> < value>
* SET < var_name> < line> < collumn> < value>

## 2 - Install guide

```bash
make install
make run
```
