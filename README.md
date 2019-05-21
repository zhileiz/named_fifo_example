# Named Fifo Example

## Run Instructions
1. run `make master`
2. run `make slave`
3. Run master first in the background: `./master &`
4. Run slave second in the foregroud: `./slave`

## Expected Output:
```
SLAVE: Hello
Master Received: Message From Slave~~.
Slave Received: ACK.
[SLAVE EXIT]
Master Received: EOS.
[MASTER EXIT]
```