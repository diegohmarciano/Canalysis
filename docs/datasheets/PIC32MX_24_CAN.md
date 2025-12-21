# PIC32MX5XX/6XX/7XX

- 1. **CONTROLLER AREA NETWORK (CAN)**

**Note:** This data sheet summarizes the features of the PIC32MX5XX/6XX/7XX family of devices. It is not intended to be a comprehensive reference source. To complement the information in this data sheet, refer to **Section 34. "Controller Area Network (CAN)"** (DS60001154) in the _"PIC32 Family Reference Manual"_, which is available from the Microchip web site ([www.microchip.com/PIC32](http://www.microchip.com/PIC32)).

The Controller Area Network (CAN) module supports the following key features:

- - - Standards Compliance:
            - Full CAN 2.0B compliance
            - Programmable bit rate up to 1 Mbps
      - Message Reception and Transmission:
        - 32 message FIFOs
        - Each FIFO can have up to 32 messages for a total of 1024 messages
        - FIFO can be a transmit message FIFO or a receive message FIFO
        - User-defined priority levels for message FIFOs used for transmission
        - 32 acceptance filters for message filtering
        - Four acceptance filter mask registers for message filtering
        - Automatic response to remote transmit request
        - DeviceNet™ addressing support
      - Additional Features:
        - Loopback, Listen All Messages, and Listen Only modes for self-test, system diagnostics and bus monitoring
        - Low-power operating modes
        - CAN module is a bus master on the PIC32 system bus
        - Use of DMA is not required
        - Dedicated time-stamp timer
        - Dedicated DMA channels
        - Data-only Message Reception mode

[Figure 24-1](#_bookmark0) illustrates the general structure of the CAN module.

Up to 32 Message Buffers

## FIGURE 24-1: PIC32 CAN MODULE BLOCK DIAGRAM

![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACUAAAAZCAYAAAC2JufVAAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAACWElEQVRIic3WQWjaUBgH8BdTsaJzg0VW0B0SQbwlGRNkagsT9N7NebIMdt5uHvTkZT3s1p570JxWdfcMhthuZYgTzWkoUw+JIFXRqdnKovl2mcWNMtjWLvnDn5fLS355fJAgn8/3vlAoPAAApJcahsPhzVar5UI6ioEkyU6n0yG1hqzGQFFUW3cokiQ7oijens/na1pjzlMul704jiscx8W1HvBlEQCgUCj0xuPxfJzP57jWoHNUqVTaQgjB4eHhI61BAICwHxfY5ubm8WQysdVqNdZgMKhajhSeTqcRhmGIoqi2IAjMdDq1er3eD1qifjo2nucj6+vrX6vV6h3NZ2q1yWRy1+VyfRqPx9d1g1IUZS0QCLyNRqM5VVWx/w26EAUASBRFJ0EQ/VwuF9UNCgBQNpvdMZlMZ8lkcldRlDVdoAAAVSqVuyRJtoPB4LEkSQ5doAAAjUajG9vb268IgujzPB/RBQoAkKqq2N7e3jOTyXQWj8e5Xq93S3PUso1Gwx0Oh1/bbLbP+/v7T69q1v54g6qqWD6ff+h0OkWapusnJyf3JpPJNU1Ry06nU2sikXhhNBq/mc3mL7FY7CXP85HL+NP457c6PT21HxwcPPH7/e8QQuBwOKRUKvW80Wi4//aeGABc2ne02Wy6M5nMY47jdrrdrsPtdjdZlq0xDFOnaVpgGKa+sbHRwzDstw+9VNQyi8UCLxaL94+OjrYEQaDr9TojSZITIYTsdnufZdkaRVFtq9U6s1gs8q/rlaAuymAwIJZAQRDofr9vl2XZIsuyZTabWVfX717dLM07d057AAAAAElFTkSuQmCC)![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABgAAAANCAYAAACzbK7QAAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAABXklEQVQ4jWO0sLA4/uHDBwFlZeW76FhBQeEBBwfHDwYKAOOmTZt8b9++rXr37l1lGH7w4IHCnz9/WBgZGf9LS0s/1dHRuRIYGLg+MDBwvaio6GuSLPj//z+G4J8/f1geP34sC7PwxIkTFhs3bvT/+PEjv6Oj4/7Q0NDVgYGB68XExF4RtOH///9E4Z8/f7Jt27bNMzExcZ6goOA7Jiamv05OTnsXLFgQj08f0RYg41+/frFu377dIykpaa6AgMD7qqqq1l+/frFSzQJkfP78eQNNTc1rpqampy5evKhHdQv+///P8PXrV66MjIzpDAwM/8vLyzv+/PnDTFULYHjevHmJbGxsPwMDA9d+/fqVk+oW/P//n+HYsWOWDg4O+2pqapppYsH///8ZDh48aMfOzv7jzp07yljzATVAYGDgOnd3951MNDGdgYHh79+/LDt27PCkmQVubm67Pnz4IAAAwuQrWw1/2AMAAAAASUVORK5CYII=)![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACYAAAAZCAYAAABdEVzWAAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAACW0lEQVRIiWP4//8/w2DCixcvjrGysjrKxDDIwO3bt1XfvXsnNOgcdu/ePSUlJaV7ow4jBnz//p3z5s2b6oPOYbNnz059+/atsL29/cEBz4Uw/O3bN05JSclnfn5+G////88w4A6C4YkTJ+YxMDD8P3funOGgcRgstAICAtbDxAbcUf///2fYtm2bJxMT098LFy7oDxqHXbt2TZOfn/9DXFzcQmTxAXXU69evRZSUlO7a29sf+P79O8egcNiPHz/YbW1tD+np6V388OEDP7r8gDjq8+fPPAEBAevl5eUfPH36VAqbGro76saNG+paWlpXpaSknt68eVMNlzq6OmrdunWBvLy8nzw9Pbe9evVKFJ9aujjo9+/fLJWVlW1sbGw/e3p6iv/+/ctESA/NHbV//34HXV3dS4qKivdOnjxpRqw+mjno0aNHsuHh4StYWVl/lZaWdn369ImXFP1Ud9C7d+8EW1paqjk5Ob95eXltxZfAae6wr1+/ci1dujTKzc1tJzMz8x8VFZXbW7Zs8abETLI1/v37l+nAgQP2iYmJ83h4eD4zMzP/8fHx2bxq1arQnz9/slHqWcb///8TbMD9/v2b9caNGxqXLl3Su3Tpkt7Fixf1f//+zbJv3z5nIyOjc3FxcYsiIyOXi4mJvaJWo5Fx7dq1QV+/fuVGxl++fOH5+vUrNycn57c9e/a4Xrt2Tev379+sLCwsfzQ1Na/r6eldMjY2PuPq6rpHR0fnCrUcg+IwFhaW39zc3F+xYQ0NjessLCx/9fX1L+rp6V3S1NS8zs7O/pMWDkEHANP8sVgiH8hZAAAAAElFTkSuQmCC)![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABgAAAANCAYAAACzbK7QAAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAABdElEQVQ4jbWSMWvCQBTHz4QoISnBT3DDRTpkFKGTFAy0tGYopJm7KXQUCn6BgkM3od3cIzjUGhAahC6CdMwizRXuCxRCDq2pzXWqVKpFqz74w93wfr/3jkswxsAmFUVRkhACfd9XMcYIY4y+z4qiBIl1BWEY7rXbbaPX6x26rqsTQmAcxxzP858QQoIQwqqq+gghnMlkXlYSUErlTqdzatu25TjOCcdxsWEY9/l8/gkh9IoQwhBCIgjCx69mxtjSOI5zbJpmUxTFkSiKI9M0m7Ztn1NKpb/6fmbhBuPxWKxUKjfD4XBfUZTAsiy7WCw+yLJM13rPRRt4nqdpmublcrmB7/to1UmXZe7SarXOJEmipVLpbjKZJDeFzwlqtdoVAICVy+XbKIqEbcBngn6/f5BKpd4bjcZFHMeJbcEZYwBMp1M+m80+V6vV622CZ4J6vX6ZTqffwjCUdyHgut3uUaFQcP/1BVcoLggCRdf1x13AAQDgC6c+O0baoDmkAAAAAElFTkSuQmCC)

System Bus

Message Buffer Size 2 or 4 Words

Message Buffer 1

Message Buffer 0

CAN Message FIFO (up to 32 FIFOs)

FIFO31

FIFO1

FIFO0

Message Buffer 31

System RAM

![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAB4AAAARCAYAAADKZhx3AAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAABbUlEQVRIiWP8//8/Az3A/fv3Fbdv3+65a9cut1OnTpkx/P//nyb4x48f7Hv27HEuKirq1dTUvMbAwPAfhjk5Ob9R1bJHjx7Jzpw5M83f338DNzf3F2TLMDAlFv369Yv1wIED9mVlZZ06OjqX8VpEqcVPnz6Vmjt3blJwcPAaXl7eTyRZRorFv3//Zjl8+LBNVVVVq4GBwXmyLSLG4hcvXogvXLgwLjw8fIWAgMB7qlmGbvGfP3+Yjx8/blFXV9dobGx8hiYWoeOtW7d6qaur36CLZQwM//n5+T/Y29sfgPv42LFjljU1Nc108zG2OH7+/LnE/PnzE8LCwlbSNI4JpepDhw7ZVlZWtunr61+gm8Xo+MmTJ9Jz5sxJDgoKWkvTfEyo5Nq/f79DaWlpl7a29hW6WYyOHz58KDdjxox0Pz+/jTQtq/HhHz9+sO/evdulsLCwT0ND4zq6xYz///9noAe4f/++4rZt27xg9TEA4sFgeKHJyrcAAAAASUVORK5CYII=)![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAB8AAAARCAYAAAAlpHdJAAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAABgUlEQVRIiWOUkJB4bmJicsbZ2Xmvh4fHDnV19ZuMjIz/GegE/iNjOTm5h6mpqbPWrl0b9P79e4H///8z0AozQi3FCpiZmf9aWFic8PDw2OHu7r7TyMjoHDMz81+a+RwfFhYWfhMZGblswYIF8c+ePZOk1OckWY6O9fT0LpaVlXXu3bvX6cePH+x0tRwZc3FxffX29t4yefLknFu3bqn++/ePkW6Wo2NFRcV7mZmZ0zZs2OD/8eNHPrpajoxZWFh+29nZHWxra6s8e/as0d+/f5n+///PwGBvb39ASEjoLT0cAcM6OjqXt2/f7kFXn7e2tladOXPGGO5zWlmooKBwPyMjY/r69esDaB7nXFxcX728vLZOmjQp9+bNm2o0T+26urqXSktLu/bs2eNM83wuJCT0NiIiYvn8+fMTnj59KkVpCcfCgAcwMzP/NTc3Pwkr242Njc9StWzn5OT8huw7WVnZRykpKbPXrFkTTPNaTUJC4rmxsfFZWH2uoaFxg171OQCrqe92AexK2gAAAABJRU5ErkJggg==)![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACMAAAAiCAYAAADVhWD8AAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAAFAklEQVRYhc3Yf0gbdxQA8He5S2KMFmeMaYyRaJeRVVJiA0rX0ZFMRXBMQWqgzokzDpxQOqGglK4rgsLAri12ZVU6ndvQKhhaQbRthu1WWlCLJWvakFZt2jQmpuLa+OPukrc/tkBnq80vpV94/9z78u5z3+/x+N4RAIDwlgwKAECj0UwyDMNrbm4+mpCQ4NtKwNTU1K4jR4600DTNAwDAkydPHhSLxW6tVjvu9XqTERG2IkZHRwsEAsGSVqsdh/92CE0mU4nFYsmWSCQujUZz2+PxpGw2ZGhoqJjP56+UlpYO9vf3l/0Pg4hgtVpVUqnUqVar77jdbvFmQQYHB0u5XC5dXl7eR9M012QylbyCQUSw2WxKmUz2eOfOnX+5XC5JrCF9fX3lJEmylZWVPzMMQyEirItBRLDb7TvkcvkjlUplffLkSVqsID09PZ9xOBx/TU1NJ8uyZPD6hhhEhOnpaYVCoZhWKpU2h8ORHi2ks7OzhiCIQF1d3Q9+v5/zcu6NGESE2dnZjKysrAdZWVkPZmdnMyKFnDlz5isAwEOHDn0fCASItfmQMIgIDocjXalU2hQKxfTDhw8zw4WcOHHiawDAxsbG1tdBwsIgIjidTqlKpbLK5fJHdrt9R6iQ1tbWRgDAY8eOfbseJGwMIoLL5ZJkZ2dbZDLZ4/v377+30dxAIEAcP378GwDAlpaWpjfVDhuDiOB2u8VqtfqOVCp1Wq1W1XqQpqamFgDAtra2hlDqRoRBRPB4PCkajea2RCJxWSyW7LWQhoaGNgDA9vb2+lBrRoxBRPB6vclarXY8JSXFMzU1tQsRwe/3c+rr69sJggh0dHQYw6kXFQYRYWFhISk3N/eWSCSaHx8f3200Gjs4HI6/u7v783BrRY1BRFhcXNy2Z8+eP3k83ipJkmxvb68hkjpBDCeas4hAIFiWyWROhmG4fD5/VaFQzERTL2IMTdM8g8HQd/HixU8vXLiwPy8v71ZBQcHlGzdufBANKOxtWl5ejisuLh6Ki4tbHhkZKURE8Pl88fn5+ZeFQuGLsbGxfVvyzvh8vvjCwsIRoVD4wmw2617OLS0tCYqKiobj4+N9a3Mxxzx//jxBp9OZExMT/75+/fqHb1q10dHRgk3BLC4ubtu7d+8fSUlJCzdv3szbaO7Kygq/pKTExOfzV4aHh4tiinn27Nk7ubm5t5KTk70TExO7Q3na1dVVXllZ2QCPx1u9dOnSJzHBzM/Pi3JycibFYrE72G1DDZqmuQaDoZfL5dIb3SMkzNzcXKparb6zffv2p3fv3n0/HEgwGIahKioqfqEoihkYGCiLCBM8w6SnpztsNpsyEkgwWJYlq6qqutbr0kEM9brG43A45Hq93syyLHXt2rV9mZmZ09E0MpIk/efPn/+Coij2wIEDv7EsS1VUVPy6dt4rmJmZGYVOp/udJEn/2NjYRxkZGY+igQQHh8MJnDt37kuKotjKysoelmWpqqqq7nUxdrv9Xb1ebxYKhb6rV69+nJaW5owF5GXQ2bNn67hcLlNdXf0TwzBco9HY+Qrm3r17Kr1ebxaJRN4rV67kSySSuVhCgoMgCDx9+vRBiqLY2traDpZlKalU+jSYx1OnTh1MTU2dy8nJmdyK72zEf0+Fhw8f/g4AsLa29kcAQAIAUKPR3KZpmtfc3Hw0MTHxxWasyOsGIkJXV1d1f3//fpZlKQLeop9F/wAkMyIlPRc3QAAAAABJRU5ErkJggg==)![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACIAAAAiCAYAAAA6RwvCAAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAADSElEQVRYhc3WX0hTURgA8O9ct927zb9vPQS9+uSDmxFiFLot1lBRnDMTW2TEGIjIQCQ1u0kEITGaQ0TU0MyxiphDdGso/kF0s4de9FV88qW/6t2d204vLezq9N67zfrgeznn8J0f5ztwDgIADP9BSAAAiouLPyGE4jRN91AUxZ7X5pubm4U2m60/HA5TAAB4YGDAUlBQ8KW8vDywv7+vwBhDpnN1dfVKbm7ud7VaHYTfXcE+n0+7sbFRfF6YlZWV0pycnB96vX7G6/Xe/AuCMYbzwCwtLZVlZ2f/NBgMXoZhKJ/Ppz0GyTRmcXHxqlKp3KusrPSEw2ESYwxJIZnCLCwsXFMqlXvV1dUfWJaVJcZPhWCMIRQKqfLz879WVFR8TBUzPz9/XaFQ7NfU1Lw/iuAFSRcmEAiUy+Xyg9ra2neRSETKnecFSRXj9/s1FEUxdXV17pMQgiBiMXNzczqKopj6+nrX4eGhJNk6QRChmNnZ2RskSYYbGhrenIYQBTmK0Wg0/mSYmZkZPUmS4cbGxtdnIURDMMYQDAbVyTBer9cgk8nYpqam8Wg0msWnnmhIMozH46mUSqWR5ubmV3wRKUO4GJfLZZRKpRGz2TwqBJEWSAKjUCj2EEJxs9k8GovFCKE1EhAilf/E9vb2JZZlKYlEcrizs3ORZVlSbC3RELfbbTSZTK6Wlpbh5eXlslAoVFJVVeVhGEYutqbg1kxNTZmysrKiFovFmWhHMBhU5+XlfdNoNP6DgwN5xu/I5OTkLYIgYlar1RGPxxH3zgjFiIJMTEzcJggi1traauciuBitVuvjgxEMGR8fbyIIItbW1vYiGSKR6+vrJXwxgiBjY2N3EELx9vb2/rMQQjG8ISMjI3cRQnGbzfacL0IIhhdkeHj4HkIo3tHR8Uwogi/mTMjQ0NB9AMCdnZ1PxSL4YE6FDA4OPgAA3NXV9SRVBBej0+nmjmKSQpxOpwUAcE9Pz+N0IRK5trZ2mYs5EeJwOKwAgHt7ex+lE3Aa5hjEbre3AgCmabo7U4iTMNPT04Y/EKvV+hIAcF9f38NMI7gYlUoVAgCMAACXlpauFBUVfTYajW/FvpxiYmtrq5Cm6e7d3d0LKHEs/zp+AUSf9QwxJfaNAAAAAElFTkSuQmCC)

CxTX

CxRX

CAN Module

CPU

32 Filters

4 Masks

| Message Buffer 31 |     |
| --- |     | --- |
|     |     |
| Message Buffer 1 |     |
| Message Buffer 0 |     |

| Message Buffer 31 |     |
| --- |     | --- |
|     |     |
| Message Buffer 1 |     |
| Message Buffer 0 |     |

 2009-2019 Microchip Technology Inc. DS60001156K-page 245

- 1. **Control Registers**

**PIC32MX5XX/6XX/7XX**

 2009-2019 Microchip Technology Inc.

DS60001156K-page 246

## TABLE 24-1: CAN1 REGISTER SUMMARY FOR PIC32MX534F064H, PIC32MX564F064H, PIC32MX564F128H, PIC32MX575F256H, PIC32MX575F512H, PIC32MX764F128H, PIC32MX775F256H, PIC32MX775F512H, PIC32MX795F512H, PIC32MX534F064L, PIC32MX564F064L, PIC32MX564F128L, PIC32MX575F256L, PIC32MX575F512L, PIC32MX764F128L, PIC32MX775F256L, PIC32MX775F512L AND PIC32MX795F512L DEVICES

**Name(1)**

| **Virtual Address (BF88_#)** | **Register** | **Bit Range** | **Bits** |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     | **All Resets** |
| --- | --- | --- | --- |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| **31/15** | **30/14** | **29/13** | **28/12** | **27/11** | **26/10** | **25/9** | **24/8** | **23/7** | **22/6** | **21/5** | **20/4** | **19/3** | **18/2** | **17/1** | **16/0** |
| B000 | C1CON | 31:16 | -   | -   | -   | -   | ABAT | REQOP&lt;2:0&gt; |     |     | OPMOD&lt;2:0&gt; |     |     | CANCAP | -   | -   | -   | -   | 0480 |
| 15:0 | ON  | -   | SIDLE | -   | CANBUSY | -   | -   | -   | -   | -   | -   | DNCNT&lt;4:0&gt; |     |     |     |     | 0000 |
| B010 | C1CFG | 31:16 | -   | -   | -   | -   | -   | -   | -   | -   | -   | WAKFIL | -   | -   | -   | SEG2PH&lt;2:0&gt; |     |     | 0000 |
| 15:0 | SEG2PHTS | SAM | SEG1PH&lt;2:0&gt; |     |     | PRSEG&lt;2:0&gt; |     |     | SJW&lt;1:0&gt; |     | BRP&lt;5:0&gt; |     |     |     |     |     | 0000 |
| B020 | C1INT | 31:16 | IVRIE | WAKIE | CERRIE | SERRIE | RBOVIE | -   | -   | -   | -   | -   | -   | -   | MODIE | CTMRIE | RBIE | TBIE | 0000 |
| 15:0 | IVRIF | WAKIF | CERRIF | SERRIF | RBOVIF | -   | -   | -   | -   | -   | -   | -   | MODIF | CTMRIF | RBIF | TBIF | 0000 |
| B030 | C1VEC | 31:16 | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | 0000 |
| 15:0 | -   | -   | -   | FILHIT&lt;4:0&gt; |     |     |     |     | -   | ICODE&lt;6:0&gt; |     |     |     |     |     |     | 0040 |
| B040 | C1TREC | 31:16 | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | TXBO | TXBP | RXBP | TXWARN | RXWARN | EWARN | 0000 |
| 15:0 | TERRCNT&lt;7:0&gt; |     |     |     |     |     |     |     | RERRCNT&lt;7:0&gt; |     |     |     |     |     |     |     | 0000 |
| B050 | C1FSTAT | 31:16 | FIFOIP31 | FIFOIP30 | FIFOIP29 | FIFOIP28 | FIFOIP27 | FIFOIP26 | FIFOIP25 | FIFOIP24 | FIFOIP23 | FIFOIP22 | FIFOIP21 | FIFOIP20 | FIFOIP19 | FIFOIP18 | FIFOIP17 | FIFOIP16 | 0000 |
| 15:0 | FIFOIP15 | FIFOIP14 | FIFOIP13 | FIFOIP12 | FIFOIP11 | FIFOIP10 | FIFOIP9 | FIFOIP8 | FIFOIP7 | FIFOIP6 | FIFOIP5 | FIFOIP4 | FIFOIP3 | FIFOIP2 | FIFOIP1 | FIFOIP0 | 0000 |
| B060 | C1RXOVF | 31:16 | RXOVF31 | RXOVF30 | RXOVF29 | RXOVF28 | RXOVF27 | RXOVF26 | RXOVF25 | RXOVF24 | RXOVF23 | RXOVF22 | RXOVF21 | RXOVF20 | RXOVF19 | RXOVF18 | RXOVF17 | RXOVF16 | 0000 |
| 15:0 | RXOVF15 | RXOVF14 | RXOVF13 | RXOVF12 | RXOVF11 | RXOVF10 | RXOVF9 | RXOVF8 | RXOVF7 | RXOVF6 | RXOVF5 | RXOVF4 | RXOVF3 | RXOVF2 | RXOVF1 | RXOVF0 | 0000 |
| B070 | C1TMR | 31:16 | CANTS&lt;15:0&gt; |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     | 0000 |
| 15:0 | CANTSPRE&lt;15:0&gt; |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     | 0000 |
| B080 | C1RXM0 | 31:16 | SID&lt;10:0&gt; |     |     |     |     |     |     |     |     |     |     | \-- | MIDE | -   | EID&lt;17:16&gt; |     | xxxx |
| 15:0 | EID&lt;15:0&gt; |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     | xxxx |
| B090 | C1RXM1 | 31:16 | SID&lt;10:0&gt; |     |     |     |     |     |     |     |     |     |     | \-- | MIDE | -   | EID&lt;17:16&gt; |     | xxxx |
| 15:0 | EID&lt;15:0&gt; |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     | xxxx |
| B0A0 | C1RXM2 | 31:16 | SID&lt;10:0&gt; |     |     |     |     |     |     |     |     |     |     | \-- | MIDE | -   | EID&lt;17:16&gt; |     | xxxx |
| 15:0 | EID&lt;15:0&gt; |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     | xxxx |
| B0B0 | C1RXM3 | 31:16 | SID&lt;10:0&gt; |     |     |     |     |     |     |     |     |     |     | \-- | MIDE | -   | EID&lt;17:16&gt; |     | xxxx |
| 15:0 | EID&lt;15:0&gt; |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     | xxxx |
| B0C0 | C1FLTCON0 | 31:16 | FLTEN3 | MSEL3&lt;1:0&gt; |     | FSEL3&lt;4:0&gt; |     |     |     |     | FLTEN2 | MSEL2&lt;1:0&gt; |     | FSEL2&lt;4:0&gt; |     |     |     |     | 0000 |
| 15:0 | FLTEN1 | MSEL1&lt;1:0&gt; |     | FSEL1&lt;4:0&gt; |     |     |     |     | FLTEN0 | MSEL0&lt;1:0&gt; |     | FSEL0&lt;4:0&gt; |     |     |     |     | 0000 |
| B0D0 | C1FLTCON1 | 31:16 | FLTEN7 | MSEL7&lt;1:0&gt; |     | FSEL7&lt;4:0&gt; |     |     |     |     | FLTEN6 | MSEL6&lt;1:0&gt; |     | FSEL6&lt;4:0&gt; |     |     |     |     | 0000 |
| 15:0 | FLTEN5 | MSEL5&lt;1:0&gt; |     | FSEL5&lt;4:0&gt; |     |     |     |     | FLTEN4 | MSEL4&lt;1:0&gt; |     | FSEL4&lt;4:0&gt; |     |     |     |     | 0000 |
| B0E0 | C1FLTCON2 | 31:16 | FLTEN11 | MSEL11&lt;1:0&gt; |     | FSEL11&lt;4:0&gt; |     |     |     |     | FLTEN10 | MSEL10&lt;1:0&gt; |     | FSEL10&lt;4:0&gt; |     |     |     |     | 0000 |
| 15:0 | FLTEN9 | MSEL9&lt;1:0&gt; |     | FSEL9&lt;4:0&gt; |     |     |     |     | FLTEN8 | MSEL8&lt;1:0&gt; |     | FSEL8&lt;4:0&gt; |     |     |     |     | 0000 |

**Legend:** x = unknown value on Reset; - = unimplemented, read as '0'. Reset values are shown in hexadecimal.

**Note 1:** All registers in this table have corresponding CLR, SET and INV registers at their virtual addresses, plus offsets of 0x4, 0x8 and 0xC, respectively. See **Section 12.1.1 "CLR, SET and INV Registers"** for more information.

## TABLE 24-1: CAN1 REGISTER SUMMARY FOR PIC32MX534F064H, PIC32MX564F064H, PIC32MX564F128H, PIC32MX575F256H, PIC32MX575F512H, PIC32MX764F128H, PIC32MX775F256H, PIC32MX775F512H, PIC32MX795F512H, PIC32MX534F064L, PIC32MX564F064L, PIC32MX564F128L, PIC32MX575F256L, PIC32MX575F512L, PIC32MX764F128L, PIC32MX775F256L, PIC32MX775F512L AND PIC32MX795F512L DEVICES (CONTINUED)

**PIC32MX5XX/6XX/7XX**

 2009-2019 Microchip Technology Inc.

DS60001156K-page 247

**Name(1)**

| **Virtual Address (BF88_#)** | **Register** | **Bit Range** | **Bits** |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     | **All Resets** |
| --- | --- | --- | --- |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| **31/15** | **30/14** | **29/13** | **28/12** | **27/11** | **26/10** | **25/9** | **24/8** | **23/7** | **22/6** | **21/5** | **20/4** | **19/3** | **18/2** | **17/1** | **16/0** |
| B0F0 | C1FLTCON3 | 31:16 | FLTEN15 | MSEL15&lt;1:0&gt; |     | FSEL15&lt;4:0&gt; |     |     |     |     | FLTEN14 | MSEL14&lt;1:0&gt; |     | FSEL14&lt;4:0&gt; |     |     |     |     | 0000 |
| 15:0 | FLTEN13 | MSEL13&lt;1:0&gt; |     | FSEL13&lt;4:0&gt; |     |     |     |     | FLTEN12 | MSEL12&lt;1:0&gt; |     | FSEL12&lt;4:0&gt; |     |     |     |     | 0000 |
| B100 | C1FLTCON4 | 31:16 | FLTEN19 | MSEL19&lt;1:0&gt; |     | FSEL19&lt;4:0&gt; |     |     |     |     | FLTEN18 | MSEL18&lt;1:0&gt; |     | FSEL18&lt;4:0&gt; |     |     |     |     | 0000 |
| 15:0 | FLTEN17 | MSEL17&lt;1:0&gt; |     | FSEL17&lt;4:0&gt; |     |     |     |     | FLTEN16 | MSEL16&lt;1:0&gt; |     | FSEL16&lt;4:0&gt; |     |     |     |     | 0000 |
| B110 | C1FLTCON5 | 31:16 | FLTEN23 | MSEL23&lt;1:0&gt; |     | FSEL23&lt;4:0&gt; |     |     |     |     | FLTEN22 | MSEL22&lt;1:0&gt; |     | FSEL22&lt;4:0&gt; |     |     |     |     | 0000 |
| 15:0 | FLTEN21 | MSEL21&lt;1:0&gt; |     | FSEL21&lt;4:0&gt; |     |     |     |     | FLTEN20 | MSEL20&lt;1:0&gt; |     | FSEL20&lt;4:0&gt; |     |     |     |     | 0000 |
| B120 | C1FLTCON6 | 31:16 | FLTEN27 | MSEL27&lt;1:0&gt; |     | FSEL27&lt;4:0&gt; |     |     |     |     | FLTEN26 | MSEL26&lt;1:0&gt; |     | FSEL26&lt;4:0&gt; |     |     |     |     | 0000 |
| 15:0 | FLTEN25 | MSEL25&lt;1:0&gt; |     | FSEL25&lt;4:0&gt; |     |     |     |     | FLTEN24 | MSEL24&lt;1:0&gt; |     | FSEL24&lt;4:0&gt; |     |     |     |     | 0000 |
| B130 | C1FLTCON7 | 31:16 | FLTEN31 | MSEL31&lt;1:0&gt; |     | FSEL31&lt;4:0&gt; |     |     |     |     | FLTEN30 | MSEL30&lt;1:0&gt; |     | FSEL30&lt;4:0&gt; |     |     |     |     | 0000 |
| 15:0 | FLTEN29 | MSEL29&lt;1:0&gt; |     | FSEL29&lt;4:0&gt; |     |     |     |     | FLTEN28 | MSEL28&lt;1:0&gt; |     | FSEL28&lt;4:0&gt; |     |     |     |     | 0000 |
| B140 | C1RXFn (n = 0-31) | 31:16 | SID&lt;10:0&gt; |     |     |     |     |     |     |     |     |     |     | \-- | EXID | -   | EID&lt;17:16&gt; |     | xxxx |
| 15:0 | EID&lt;15:0&gt; |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     | xxxx |
| B340 | C1FIFOBA | 31:16<br><br>15:0 | C1FIFOBA&lt;31:0&gt; |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     | 0000 |
| 0000 |
| B350 | C1FIFOCONn (n = 0-31) | 31:16 | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   |     | FSIZE&lt;4:0&gt; |     |     |     | 0000 |
| 15:0 | -   | FRESET | UINC | DONLY | -   | -   | -   | -   | TXEN | TXABAT | TXLARB | TXERR | TXREQ | RTREN | TXPRI&lt;1:0&gt; |     | 0000 |
| B360 | C1FIFOINTn (n = 0-31) | 31:16 | -   | -   | -   | -   | -   | TXNFULLIE | TXHALFIE | TXEMPTYIE | -   | -   | -   | -   | RXOVFLIE | RXFULLIE | RXHALFIE | RXN EMPTYIE | 0000 |
| 15:0 | -   | -   | -   | -   | -   | TXNFULLIF | TXHALFIF | TXEMPTYIF | -   | -   | -   | -   | RXOVFLIF | RXFULLIF | RXHALFIF | RXN EMPTYIF | 0000 |
| B370 | C1FIFOUAn (n = 0-31) | 31:16 | C1FIFOUA&lt;31:0&gt; |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     | 0000 |
| 15:0 | 0000 |
| B380 | C1FIFOCIn (n = 0-31) | 31:16 | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | 0000 |
| 15:0 | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | C1FIFOCI&lt;4:0&gt; |     |     |     |     | 0000 |

**Legend:** x = unknown value on Reset; - = unimplemented, read as '0'. Reset values are shown in hexadecimal.

**Note 1:** All registers in this table have corresponding CLR, SET and INV registers at their virtual addresses, plus offsets of 0x4, 0x8 and 0xC, respectively. See **Section 12.1.1 "CLR, SET and INV Registers"** for more information.

## TABLE 24-2: CAN2 REGISTER SUMMARY FOR PIC32MX775F256H, PIC32MX775F512H, PIC32MX795F512H, PIC32MX775F256L, PIC32MX775F512L AND PIC32MX795F512L DEVICES

**PIC32MX5XX/6XX/7XX**

 2009-2019 Microchip Technology Inc.

DS60001156K-page 248

**Name(1)**

| **Virtual Address (BF88_#)** | **Register** | **Bit Range** | **Bits** |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     | **All Resets** |
| --- | --- | --- | --- |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| **31/15** | **30/14** | **29/13** | **28/12** | **27/11** | **26/10** | **25/9** | **24/8** | **23/7** | **22/6** | **21/5** | **20/4** | **19/3** | **18/2** | **17/1** | **16/0** |
| C000 | C2CON | 31:16 | -   | -   | -   | -   | ABAT | REQOP&lt;2:0&gt; |     |     | OPMOD&lt;2:0&gt; |     |     | CANCAP | -   | -   | -   | -   | 0480 |
| 15:0 | ON  | -   | SIDLE | -   | CANBUSY | -   | -   | -   | -   | -   | -   | DNCNT&lt;4:0&gt; |     |     |     |     | 0000 |
| C010 | C2CFG | 31:16 | -   | -   | -   | -   | -   | -   | -   | -   | -   | WAKFIL | -   | -   | -   | SEG2PH&lt;2:0&gt; |     |     | 0000 |
| 15:0 | SEG2PHTS | SAM | SEG1PH&lt;2:0&gt; |     |     | PRSEG&lt;2:0&gt; |     |     | SJW&lt;1:0&gt; |     | BRP&lt;5:0&gt; |     |     |     |     |     | 0000 |
| C020 | C2INT | 31:16 | IVRIE | WAKIE | CERRIE | SERRIE | RBOVIE | -   | -   | -   | -   | -   | -   | -   | MODIE | CTMRIE | RBIE | TBIE | 0000 |
| 15:0 | IVRIF | WAKIF | CERRIF | SERRIF | RBOVIF | -   | -   | -   | -   | -   | -   | -   | MODIF | CTMRIF | RBIF | TBIF | 0000 |
| C030 | C2VEC | 31:16 | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | 0000 |
| 15:0 | -   | -   | -   | FILHIT&lt;4:0&gt; |     |     |     |     | -   | ICODE&lt;6:0&gt; |     |     |     |     |     |     | 0040 |
| C040 | C2TREC | 31:16 | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | TXBO | TXBP | RXBP | TXWARN | RXWARN | EWARN | 0000 |
| 15:0 | TERRCNT&lt;7:0&gt; |     |     |     |     |     |     |     | RERRCNT&lt;7:0&gt; |     |     |     |     |     |     |     | 0000 |
| C050 | C2FSTAT | 31:16 | FIFOIP31 | FIFOIP30 | FIFOIP29 | FIFOIP28 | FIFOIP27 | FIFOIP26 | FIFOIP25 | FIFOIP24 | FIFOIP23 | FIFOIP22 | FIFOIP21 | FIFOIP20 | FIFOIP19 | FIFOIP18 | FIFOIP17 | FIFOIP16 | 0000 |
| 15:0 | FIFOIP15 | FIFOIP14 | FIFOIP13 | FIFOIP12 | FIFOIP11 | FIFOIP10 | FIFOIP9 | FIFOIP8 | FIFOIP7 | FIFOIP6 | FIFOIP5 | FIFOIP4 | FIFOIP3 | FIFOIP2 | FIFOIP1 | FIFOIP0 | 0000 |
| C060 | C2RXOVF | 31:16 | RXOVF31 | RXOVF30 | RXOVF29 | RXOVF28 | RXOVF27 | RXOVF26 | RXOVF25 | RXOVF24 | RXOVF23 | RXOVF22 | RXOVF21 | RXOVF20 | RXOVF19 | RXOVF18 | RXOVF17 | RXOVF16 | 0000 |
| 15:0 | RXOVF15 | RXOVF14 | RXOVF13 | RXOVF12 | RXOVF11 | RXOVF10 | RXOVF9 | RXOVF8 | RXOVF7 | RXOVF6 | RXOVF5 | RXOVF4 | RXOVF3 | RXOVF2 | RXOVF1 | RXOVF0 | 0000 |
| C070 | C2TMR | 31:16 | CANTS&lt;15:0&gt; |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     | 0000 |
| 15:0 | CANTSPRE&lt;15:0&gt; |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     | 0000 |
| C080 | C2RXM0 | 31:16 | SID&lt;10:0&gt; |     |     |     |     |     |     |     |     |     |     | \-- | MIDE | -   | EID&lt;17:16&gt; |     | xxxx |
| 15:0 | EID&lt;15:0&gt; |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     | xxxx |
| C0A0 | C2RXM1 | 31:16 | SID&lt;10:0&gt; |     |     |     |     |     |     |     |     |     |     | \-- | MIDE | -   | EID&lt;17:16&gt; |     | xxxx |
| 15:0 | EID&lt;15:0&gt; |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     | xxxx |
| C0B0 | C2RXM2 | 31:16 | SID&lt;10:0&gt; |     |     |     |     |     |     |     |     |     |     | \-- | MIDE | -   | EID&lt;17:16&gt; |     | xxxx |
| 15:0 | EID&lt;15:0&gt; |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     | xxxx |
| C0B0 | C2RXM3 | 31:16 | SID&lt;10:0&gt; |     |     |     |     |     |     |     |     |     |     | \-- | MIDE | -   | EID&lt;17:16&gt; |     | xxxx |
| 15:0 | EID&lt;15:0&gt; |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     | xxxx |
| C0C0 | C2FLTCON0 | 31:16 | FLTEN3 | MSEL3&lt;1:0&gt; |     | FSEL3&lt;4:0&gt; |     |     |     |     | FLTEN2 | MSEL2&lt;1:0&gt; |     | FSEL2&lt;4:0&gt; |     |     |     |     | 0000 |
| 15:0 | FLTEN1 | MSEL1&lt;1:0&gt; |     | FSEL1&lt;4:0&gt; |     |     |     |     | FLTEN0 | MSEL0&lt;1:0&gt; |     | FSEL0&lt;4:0&gt; |     |     |     |     | 0000 |
| C0D0 | C2FLTCON1 | 31:16 | FLTEN7 | MSEL7&lt;1:0&gt; |     | FSEL7&lt;4:0&gt; |     |     |     |     | FLTEN6 | MSEL6&lt;1:0&gt; |     | FSEL6&lt;4:0&gt; |     |     |     |     | 0000 |
| 15:0 | FLTEN5 | MSEL5&lt;1:0&gt; |     | FSEL5&lt;4:0&gt; |     |     |     |     | FLTEN4 | MSEL4&lt;1:0&gt; |     | FSEL4&lt;4:0&gt; |     |     |     |     | 0000 |
| C0E0 | C2FLTCON2 | 31:16 | FLTEN11 | MSEL11&lt;1:0&gt; |     | FSEL11&lt;4:0&gt; |     |     |     |     | FLTEN10 | MSEL10&lt;1:0&gt; |     | FSEL10&lt;4:0&gt; |     |     |     |     | 0000 |
| 15:0 | FLTEN9 | MSEL9&lt;1:0&gt; |     | FSEL9&lt;4:0&gt; |     |     |     |     | FLTEN8 | MSEL8&lt;1:0&gt; |     | FSEL8&lt;4:0&gt; |     |     |     |     | 0000 |
| C0F0 | C2FLTCON3 | 31:16 | FLTEN15 | MSEL15&lt;1:0&gt; |     | FSEL15&lt;4:0&gt; |     |     |     |     | FLTEN14 | MSEL14&lt;1:0&gt; |     | FSEL14&lt;4:0&gt; |     |     |     |     | 0000 |
| 15:0 | FLTEN13 | MSEL13&lt;1:0&gt; |     | FSEL13&lt;4:0&gt; |     |     |     |     | FLTEN12 | MSEL12&lt;1:0&gt; |     | FSEL12&lt;4:0&gt; |     |     |     |     | 0000 |

**Legend:** x = unknown value on Reset; - = unimplemented, read as '0'. Reset values are shown in hexadecimal.

**Note 1:** All registers in this table have corresponding CLR, SET and INV registers at their virtual addresses, plus offsets of 0x4, 0x8 and 0xC, respectively. See **Section 12.1.1 "CLR, SET and INV Registers"** for more information.

## TABLE 24-2: CAN2 REGISTER SUMMARY FOR PIC32MX775F256H, PIC32MX775F512H, PIC32MX795F512H, PIC32MX775F256L, PIC32MX775F512L AND PIC32MX795F512L DEVICES (CONTINUED)

**PIC32MX5XX/6XX/7XX**

 2009-2019 Microchip Technology Inc.

DS60001156K-page 249

**Name(1)**

| **Virtual Address (BF88_#)** | **Register** | **Bit Range** | **Bits** |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     | **All Resets** |
| --- | --- | --- | --- |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| **31/15** | **30/14** | **29/13** | **28/12** | **27/11** | **26/10** | **25/9** | **24/8** | **23/7** | **22/6** | **21/5** | **20/4** | **19/3** | **18/2** | **17/1** | **16/0** |
| C100 | C2FLTCON4 | 31:16 | FLTEN19 | MSEL19&lt;1:0&gt; |     | FSEL19&lt;4:0&gt; |     |     |     |     | FLTEN18 | MSEL18&lt;1:0&gt; |     | FSEL18&lt;4:0&gt; |     |     |     |     | 0000 |
| 15:0 | FLTEN17 | MSEL17&lt;1:0&gt; |     | FSEL17&lt;4:0&gt; |     |     |     |     | FLTEN16 | MSEL16&lt;1:0&gt; |     | FSEL16<4:0: |     |     |     |     | 0000 |
| C110 | C2FLTCON5 | 31:16 | FLTEN23 | MSEL23&lt;1:0&gt; |     | FSEL23&lt;4:0&gt; |     |     |     |     | FLTEN22 | MSEL22&lt;1:0&gt; |     | FSEL22&lt;4:0&gt; |     |     |     |     | 0000 |
| 15:0 | FLTEN21 | MSEL21&lt;1:0&gt; |     | FSEL21&lt;4:0&gt; |     |     |     |     | FLTEN20 | MSEL20&lt;1:0&gt; |     | FSEL20&lt;4:0&gt; |     |     |     |     | 0000 |
| C120 | C2FLTCON6 | 31:16 | FLTEN27 | MSEL27&lt;1:0&gt; |     | FSEL27&lt;4:0&gt; |     |     |     |     | FLTEN26 | MSEL26&lt;1:0&gt; |     | FSEL26&lt;4:0&gt; |     |     |     |     | 0000 |
| 15:0 | FLTEN25 | MSEL25&lt;1:0&gt; |     | FSEL25&lt;4:0&gt; |     |     |     |     | FLTEN24 | MSEL24&lt;1:0&gt; |     | FSEL24&lt;4:0&gt; |     |     |     |     | 0000 |
| C130 | C2FLTCON7 | 31:16 | FLTEN31 | MSEL31&lt;1:0&gt; |     | FSEL31&lt;4:0&gt; |     |     |     |     | FLTEN30 | MSEL30&lt;1:0&gt; |     | FSEL30&lt;4:0&gt; |     |     |     |     | 0000 |
| 15:0 | FLTEN29 | MSEL29&lt;1:0&gt; |     | FSEL29&lt;4:0&gt; |     |     |     |     | FLTEN28 | MSEL28&lt;1:0&gt; |     | FSEL28&lt;4:0&gt; |     |     |     |     | 0000 |
| C140 | C2RXFn (n = 0-31) | 31:16 | SID&lt;10:0&gt; |     |     |     |     |     |     |     |     |     |     | \-- | EXID | -   | EID&lt;17:16&gt; |     | xxxx |
| 15:0 | EID&lt;15:0&gt; |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     | xxxx |
| C340 | C2FIFOBA | 31:16 | C2FIFOBA&lt;31:0&gt; |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     | 0000 |
| 15:0 | 0000 |
| C350 | C2FIFOCONn (n = 0-31) | 31:16 | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   |     | FSIZE&lt;4:0&gt; |     |     |     | 0000 |
| 15:0 | -   | FRESET | UINC | DONLY | -   | -   | -   | -   | TXEN | TXABAT | TXLARB | TXERR | TXREQ | RTREN | TXPRI&lt;1:0&gt; |     | 0000 |
| C360 | C2FIFOINTn (n = 0-31) | 31:16 | -   | -   | -   | -   | -   | TXNFULLIE | TXHALFIE | TXEMPTYIE | -   | -   | -   | -   | RXOVFLIE | RXFULLIE | RXHALFIE | RXN EMPTYIE | 0000 |
| 15:0 | -   | -   | -   | -   | -   | TXNFULLIF | TXHALFIF | TXEMPTYIF | -   | -   | -   | -   | RXOVFLIF | RXFULLIF | RXHALFIF | RXN EMPTYIF | 0000 |
| C370 | C2FIFOUAn (n = 0-31) | 31:16 | C2FIFOUA&lt;31:0&gt; |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     | 0000 |
| 15:0 | 0000 |
| C380 | C2FIFOCIn (n = 0-31) | 31:16 | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | 0000 |
| 15:0 | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | -   | C2FIFOCI&lt;4:0&gt; |     |     |     |     | 0000 |

**Legend:** x = unknown value on Reset; - = unimplemented, read as '0'. Reset values are shown in hexadecimal.

**Note 1:** All registers in this table have corresponding CLR, SET and INV registers at their virtual addresses, plus offsets of 0x4, 0x8 and 0xC, respectively. See **Section 12.1.1 "CLR, SET and INV Registers"** for more information.

### REGISTER 24-1: CiCON: CAN MODULE CONTROL REGISTER

| **Bit Range** | **Bit 31/23/15/7** | **Bit 30/22/14/6** | **Bit 29/21/13/5** | **Bit 28/20/12/4** | **Bit 27/19/11/3** | **Bit 26/18/10/2** | **Bit 25/17/9/1** | **Bit 24/16/8/0** |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 31:24 | U-0 | U-0 | U-0 | U-0 | S/HC-0 | R/W-1 | R/W-0 | R/W-0 |
| -   | -   | -   | -   | ABAT | REQOP&lt;2:0&gt; |     |     |
| 23:16 | R-1 | R-0 | R-0 | R/W-0 | U-0 | U-0 | U-0 | U-0 |
| OPMOD&lt;2:0&gt; |     |     | CANCAP | -   | -   | -   | -   |
| 15:8 | R/W-0 | U-0 | R/W-0 | U-0 | R-0 | U-0 | U-0 | U-0 |
| ON**(1)** | -   | SIDLE | -   | CANBUSY | -   | -   | -   |
| 7:0 | U-0 | U-0 | U-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| -   | -   | -   | DNCNT&lt;4:0&gt; |     |     |     |     |

| **Legend:** | HC = Hardware Clear | S = Settable bit |
| --- | --- | --- |
| R = Readable bit | W = Writable bit | U = Unimplemented bit, read as '0' |
| \-n = Value at POR | '1' = Bit is set | '0' = Bit is cleared x = Bit is unknown |

bit 31-28 **Unimplemented:** Read as '0'

bit 27 **ABAT:** Abort All Pending Transmissions bit

1 = Signal all transmit buffers to abort transmission

0 = Module will clear this bit when all transmissions aborted bit 26-24 **REQOP&lt;2:0&gt;:** Request Operation Mode bits

111 = Set Listen All Messages mode

110 = Reserved

101 = Reserved

100 = Set Configuration mode 011 = Set Listen Only mode 010 = Set Loopback mode 001 = Set Disable mode

000 = Set Normal Operation mode

bit 23-21 **OPMOD&lt;2:0&gt;:** Operation Mode Status bits

111 = Module is in Listen All Messages mode

110 = Reserved

101 = Reserved

100 = Module is in Configuration mode 011 = Module is in Listen Only mode 010 = Module is in Loopback mode 001 = Module is in Disable mode

000 = Module is in Normal Operation mode

bit 20 **CANCAP:** CAN Message Receive Time Stamp Timer Capture Enable bit

1 = CANTMR value is stored on valid message reception and is stored with the message

0 = Disable CAN message receive time stamp timer capture and stop CANTMR to conserve power bit 19-16 **Unimplemented:** Read as '0'

bit 15 **ON:** CAN On bit**(1)**

1 = CAN module is enabled

0 = CAN module is disabled bit 14 **Unimplemented:** Read as '0'

**Note 1:** If the user application clears this bit, it may take a number of cycles before the CAN module completes the current transaction and responds to this request. The user application should poll the CANBUSY bit to verify that the request has been honored.

### REGISTER 24-1: CiCON: CAN MODULE CONTROL REGISTER (CONTINUED)

bit 13 **SIDLE:** CAN Stop in Idle bit

1 = CAN Stops operation when system enters Idle mode

0 = CAN continues operation when system enters Idle mode bit 12 **Unimplemented:** Read as '0'

bit 11 **CANBUSY:** CAN Module is Busy bit

1 = The CAN module is active

0 = The CAN module is completely disabled bit 10-5 **Unimplemented:** Read as '0'

bit 4-0 **DNCNT&lt;4:0&gt;:** Device Net Filter Bit Number bits

10011-11111 = Invalid Selection (compare up to 18-bits of data with EID)

10010 = Compare up to data byte 2 bit 6 with EID17 (CiRXFn&lt;17&gt;)

•

•

•

00001 = Compare up to data byte 0 bit 7 with EID0 (CiRXFn&lt;0&gt;)

00000 = Do not compare data bytes

**Note 1:** If the user application clears this bit, it may take a number of cycles before the CAN module completes the current transaction and responds to this request. The user application should poll the CANBUSY bit to verify that the request has been honored.

### REGISTER 24-2: CiCFG: CAN BAUD RATE CONFIGURATION REGISTER

| **Bit Range** | **Bit 31/23/15/7** | **Bit 30/22/14/6** | **Bit 29/21/13/5** | **Bit 28/20/12/4** | **Bit 27/19/11/3** | **Bit 26/18/10/2** | **Bit 25/17/9/1** | **Bit 24/16/8/0** |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 31:24 | U-0 | U-0 | U-0 | U-0 | U-0 | U-0 | U-0 | U-0 |
| -   | -   | -   | -   | -   | -   | -   | -   |
| 23:16 | U-0 | R/W-0 | U-0 | U-0 | U-0 | R/W-0 | R/W-0 | R/W-0 |
| -   | WAKFIL | -   | -   | -   | SEG2PH&lt;2:0&gt;**(1,4)** |     |     |
| 15:8 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| SEG2PHTS**(1)** | SAM**(2)** | SEG1PH&lt;2:0&gt; |     |     | PRSEG&lt;2:0&gt; |     |     |
| 7:0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| SJW&lt;1:0&gt;**(3)** |     | BRP&lt;5:0&gt; |     |     |     |     |     |

| **Legend:** | HC = Hardware Clear | S = Settable bit |
| --- | --- | --- |
| R = Readable bit | W = Writable bit | U = Unimplemented bit, read as '0' |
| \-n = Value at POR | '1' = Bit is set | '0' = Bit is cleared x = Bit is unknown |

bit 31-23 **Unimplemented:** Read as '0'

bit 22 **WAKFIL:** CAN Bus Line Filter Enable bit

1 = Use CAN bus line filter for wake-up

0 = CAN bus line filter is not used for wake-up bit 21-19 **Unimplemented:** Read as '0'

bit 18-16 **SEG2PH&lt;2:0&gt;:** Phase Buffer Segment 2 bits**(1,4)**

111 = Length is 8 x TQ

•

•

•

000 = Length is 1 x TQ

bit 15 **SEG2PHTS:** Phase Segment 2 Time Select bit**(1)**

1 = Freely programmable

0 = Maximum of SEG1PH or Information Processing Time, whichever is greater bit 14 **SAM:** Sample of the CAN Bus Line bit**(2)**

1 = Bus line is sampled three times at the sample point

0 = Bus line is sampled once at the sample point bit 13-11 **SEG1PH&lt;2:0&gt;:** Phase Buffer Segment 1 bits**(4)**

111 = Length is 8 x TQ

•

•

•

000 = Length is 1 x TQ

**Note 1:** SEG2PH  SEG1PH. If SEG2PHTS is clear, SEG2PH will be set automatically.

**2:** 3 Time bit sampling is not allowed for BRP < 2.

**3:** SJW  SEG2PH.

**4:** The Time Quanta per bit must be greater than 7 (that is, TQBIT > 7).

**Note:** This register can only be modified when the CAN module is in Configuration mode (OPMOD&lt;2:0&gt; (CiCON&lt;23:21&gt;) = 100).

### REGISTER 24-2: CiCFG: CAN BAUD RATE CONFIGURATION REGISTER (CONTINUED)

bit 10-8 **PRSEG&lt;2:0&gt;:** Propagation Time Segment bits**(4)**

111 = Length is 8 x TQ

•

•

•

000 = Length is 1 x TQ

bit 7-6 **SJW&lt;1:0&gt;:** Synchronization Jump Width bits**(3)**

11 = Length is 4 x TQ 10 = Length is 3 x TQ 01 = Length is 2 x TQ 00 = Length is 1 x TQ

bit 5-0 **BRP&lt;5:0&gt;:** Baud Rate Prescaler bits

111111 = TQ = (2 x 64)/FSYS

111110 = TQ = (2 x 63)/FSYS

•

•

•

000001 = TQ = (2 x 2)/FSYS

000000 = TQ = (2 x 1)/FSYS

**Note 1:** SEG2PH  SEG1PH. If SEG2PHTS is clear, SEG2PH will be set automatically.

**2:** 3 Time bit sampling is not allowed for BRP < 2.

**3:** SJW  SEG2PH.

**4:** The Time Quanta per bit must be greater than 7 (that is, TQBIT > 7).

**Note:** This register can only be modified when the CAN module is in Configuration mode (OPMOD&lt;2:0&gt; (CiCON&lt;23:21&gt;) = 100).

### REGISTER 24-3: CiINT: CAN INTERRUPT REGISTER

| **Bit Range** | **Bit 31/23/15/7** | **Bit 30/22/14/6** | **Bit 29/21/13/5** | **Bit 28/20/12/4** | **Bit 27/19/11/3** | **Bit 26/18/10/2** | **Bit 25/17/9/1** | **Bit 24/16/8/0** |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 31:24 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | U-0 | U-0 | U-0 |
| IVRIE | WAKIE | CERRIE | SERRIE | RBOVIE | -   | -   | -   |
| 23:16 | U-0 | U-0 | U-0 | U-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| -   | -   | -   | -   | MODIE | CTMRIE | RBIE | TBIE |
| 15:8 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | U-0 | U-0 | U-0 |
| IVRIF | WAKIF | CERRIF | SERRIF**(1)** | RBOVIF | -   | -   | -   |
| 7:0 | U-0 | U-0 | U-0 | U-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| -   | -   | -   | -   | MODIF | CTMRIF | RBIF | TBIF |

| **Legend:** |     |     |
| --- | --- |     | --- |
| R = Readable bit | W = Writable bit | U = Unimplemented bit, read as '0' |
| \-n = Value at POR | '1' = Bit is set | '0' = Bit is cleared x = Bit is unknown |

bit 31 **IVRIE:** Invalid Message Received Interrupt Enable bit

1 = Interrupt request is enabled

0 = Interrupt request is not enabled

bit 30 **WAKIE:** CAN Bus Activity Wake-up Interrupt Enable bit

1 = Interrupt request is enabled

0 = Interrupt request is not enabled

bit 29 **CERRIE:** CAN Bus Error Interrupt Enable bit

1 = Interrupt request is enabled

0 = Interrupt request is not enabled

bit 28 **SERRIE:** System Error Interrupt Enable bit

1 = Interrupt request is enabled

0 = Interrupt request is not enabled

bit 27 **RBOVIE:** Receive Buffer Overflow Interrupt Enable bit

1 = Interrupt request is enabled

0 = Interrupt request is not enabled bit 26-20 **Unimplemented:** Read as '0'

bit 19 **MODIE:** Mode Change Interrupt Enable bit

1 = Interrupt request is enabled

0 = Interrupt request is not enabled

bit 18 **CTMRIE:** CAN Timestamp Timer Interrupt Enable bit

1 = Interrupt request is enabled

0 = Interrupt request is not enabled

bit 17 **RBIE:** Receive Buffer Interrupt Enable bit

1 = Interrupt request is enabled

0 = Interrupt request is not enabled

bit 16 **TBIE:** Transmit Buffer Interrupt Enable bit

1 = Interrupt request is enabled

0 = Interrupt request is not enabled

bit 15 **IVRIF:** Invalid Message Received Interrupt Flag bit

1 = An invalid messages interrupt has occurred

0 = An invalid message interrupt has not occurred

**Note 1:** This bit can only be cleared by turning the CAN module Off and On by clearing or setting the ON bit (CiCON&lt;15&gt;).

### REGISTER 24-3: CiINT: CAN INTERRUPT REGISTER (CONTINUED)

bit 14 **WAKIF:** CAN Bus Activity Wake-up Interrupt Flag bit

1 = A bus wake-up activity interrupt has occurred

0 = A bus wake-up activity interrupt has not occurred bit 13 **CERRIF:** CAN Bus Error Interrupt Flag bit

1 = A CAN bus error has occurred

0 = A CAN bus error has not occurred bit 12 **SERRIF:** System Error Interrupt Flag bit

1 = A system error occurred (typically an illegal address was presented to the system bus)

0 = A system error has not occurred

bit 11 **RBOVIF:** Receive Buffer Overflow Interrupt Flag bit

1 = A receive buffer overflow has occurred

0 = A receive buffer overflow has not occurred bit 10-4 **Unimplemented:** Read as '0'

bit 3 **MODIF:** CAN Mode Change Interrupt Flag bit

1 = A CAN module mode change has occurred (OPMOD&lt;2:0&gt; has changed to reflect REQOP)

0 = A CAN module mode change has not occurred bit 2 **CTMRIF:** CAN Timer Overflow Interrupt Flag bit

1 = A CAN timer (CANTMR) overflow has occurred

0 = A CAN timer (CANTMR) overflow has not occurred bit 1 **RBIF:** Receive Buffer Interrupt Flag bit

1 = A receive buffer interrupt is pending

0 = A receive buffer interrupt is not pending bit 0 **TBIF:** Transmit Buffer Interrupt Flag bit

1 = A transmit buffer interrupt is pending

0 = A transmit buffer interrupt is not pending

**Note 1:** This bit can only be cleared by turning the CAN module Off and On by clearing or setting the ON bit (CiCON&lt;15&gt;).

### REGISTER 24-4: CiVEC: CAN INTERRUPT CODE REGISTER

| **Bit Range** | **Bit 31/23/15/7** | **Bit 30/22/14/6** | **Bit 29/21/13/5** | **Bit 28/20/12/4** | **Bit 27/19/11/3** | **Bit 26/18/10/2** | **Bit 25/17/9/1** | **Bit 24/16/8/0** |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 31:24 | U-0 | U-0 | U-0 | U-0 | U-0 | U-0 | U-0 | U-0 |
| -   | -   | -   | -   | -   | -   | -   | -   |
| 23:16 | U-0 | U-0 | U-0 | U-0 | U-0 | U-0 | U-0 | U-0 |
| -   | -   | -   | -   | -   | -   | -   | -   |
| 15:8 | U-0 | U-0 | U-0 | R-0 | R-0 | R-0 | R-0 | R-0 |
| -   | -   | -   | FILHIT&lt;4:0&gt; |     |     |     |     |
| 7:0 | U-0 | R-1 | R-0 | R-0 | R-0 | R-0 | R-0 | R-0 |
| -   | ICODE&lt;6:0&gt;**(1)** |     |     |     |     |     |     |

| **Legend:** |     |     |
| --- | --- |     | --- |
| R = Readable bit | W = Writable bit | U = Unimplemented bit, read as '0' |
| \-n = Value at POR | '1' = Bit is set | '0' = Bit is cleared x = Bit is unknown |

bit 31-13 **Unimplemented:** Read as '0'

bit 12-8 **FILHIT&lt;4:0&gt;:** Filter Hit Number bit

11111 = Filter 31

11110 = Filter 30

•

•

•

00001 = Filter 1

00000 = Filter 0

bit 7 **Unimplemented:** Read as '0'

bit 6-0 **ICODE&lt;6:0&gt;:** Interrupt Flag Code bits**(1)**

1111111 = Reserved

•

•

•

1001001 = Reserved

1001000 = Invalid message received (IVRIF) 1000111 = CAN module mode change (MODIF) 1000110 = CAN timestamp timer (CTMRIF) 1000101 = Bus bandwidth error (SERRIF) 1000100 = Address error interrupt (SERRIF)

1000011 = Receive FIFO overflow interrupt (RBOVIF)

1000010 = Wake-up interrupt (WAKIF) 1000001 = Error Interrupt (CERRIF) 1000000 = No interrupt

0111111 = Reserved

•

•

•

0100000 = Reserved

0011111 = FIFO31 Interrupt (CiFSTAT&lt;31&gt; set)

0011110 = FIFO30 Interrupt (CiFSTAT&lt;30&gt; set)

•

•

•

0000001 = FIFO1 Interrupt (CiFSTAT&lt;1&gt; set)

0000000 = FIFO0 Interrupt (CiFSTAT&lt;0&gt; set)

**Note 1:** These bits are only updated for enabled interrupts.

### REGISTER 24-5: CiTREC: CAN TRANSMIT/RECEIVE ERROR COUNT REGISTER

| **Bit Range** | **Bit 31/23/15/7** | **Bit 30/22/14/6** | **Bit 29/21/13/5** | **Bit 28/20/12/4** | **Bit 27/19/11/3** | **Bit 26/18/10/2** | **Bit 25/17/9/1** | **Bit 24/16/8/0** |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 31:24 | U-0 | U-0 | U-0 | U-0 | U-0 | U-0 | U-0 | U-0 |
| -   | -   | -   | -   | -   | -   | -   | -   |
| 23:16 | U-0 | U-0 | R-0 | R-0 | R-0 | R-0 | R-0 | R-0 |
| -   | -   | TXBO | TXBP | RXBP | TXWARN | RXWARN | EWARN |
| 15:8 | R-0 | R-0 | R-0 | R-0 | R-0 | R-0 | R-0 | R-0 |
| TERRCNT&lt;7:0&gt; |     |     |     |     |     |     |     |
| 7:0 | R-0 | R-0 | R-0 | R-0 | R-0 | R-0 | R-0 | R-0 |
| RERRCNT&lt;7:0&gt; |     |     |     |     |     |     |     |

| **Legend:** |     |     |
| --- | --- |     | --- |
| R = Readable bit | W = Writable bit | U = Unimplemented bit, read as '0' |
| \-n = Value at POR | '1' = Bit is set | '0' = Bit is cleared x = Bit is unknown |

bit 31-22 **Unimplemented:** Read as '0'

bit 21 **TXBO:** Transmitter in Error State Bus OFF (TERRCNT  256)

bit 20 **TXBP:** Transmitter in Error State Bus Passive (TERRCNT  128) bit 19 **RXBP:** Receiver in Error State Bus Passive (RERRCNT  128)

bit 18 **TXWARN:** Transmitter in Error State Warning (128 > TERRCNT  96) bit 17 **RXWARN:** Receiver in Error State Warning (128 > RERRCNT  96) bit 16 **EWARN:** Transmitter or Receiver is in Error State Warning

bit 15-8 **TERRCNT&lt;7:0&gt;:** Transmit Error Counter bit 7-0 **RERRCNT&lt;7:0&gt;:** Receive Error Counter

### REGISTER 24-6: CiFSTAT: CAN FIFO STATUS REGISTER

| **Bit Range** | **Bit 31/23/15/7** | **Bit 30/22/14/6** | **Bit 29/21/13/5** | **Bit 28/20/12/4** | **Bit 27/19/11/3** | **Bit 26/18/10/2** | **Bit 25/17/9/1** | **Bit 24/16/8/0** |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 31:24 | R-0 | R-0 | R-0 | R-0 | R-0 | R-0 | R-0 | R-0 |
| FIFOIP31 | FIFOIP30 | FIFOIP29 | FIFOIP28 | FIFOIP27 | FIFOIP26 | FIFOIP25 | FIFOIP24 |
| 23:16 | R-0 | R-0 | R-0 | R-0 | R-0 | R-0 | R-0 | R-0 |
| FIFOIP23 | FIFOIP22 | FIFOIP21 | FIFOIP20 | FIFOIP19 | FIFOIP18 | FIFOIP17 | FIFOIP16 |
| 15:8 | R-0 | R-0 | R-0 | R-0 | R-0 | R-0 | R-0 | R-0 |
| FIFOIP15 | FIFOIP14 | FIFOIP13 | FIFOIP12 | FIFOIP11 | FIFOIP10 | FIFOIP9 | FIFOIP8 |
| 7:0 | R-0 | R-0 | R-0 | R-0 | R-0 | R-0 | R-0 | R-0 |
| FIFOIP7 | FIFOIP6 | FIFOIP5 | FIFOIP4 | FIFOIP3 | FIFOIP2 | FIFOIP1 | FIFOIP0 |

| **Legend:** |     |     |
| --- | --- |     | --- |
| R = Readable bit | W = Writable bit | U = Unimplemented bit, read as '0' |
| \-n = Value at POR | '1' = Bit is set | '0' = Bit is cleared x = Bit is unknown |

bit 31-0 **FIFOIP&lt;31:0&gt;:** FIFOn Interrupt Pending bits

1 = One or more enabled FIFO interrupts are pending

0 = No FIFO interrupts are pending

### REGISTER 24-7: CiRXOVF: CAN RECEIVE FIFO OVERFLOW STATUS REGISTER

| **Bit Range** | **Bit 31/23/15/7** | **Bit 30/22/14/6** | **Bit 29/21/13/5** | **Bit 28/20/12/4** | **Bit 27/19/11/3** | **Bit 26/18/10/2** | **Bit 25/17/9/1** | **Bit 24/16/8/0** |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 31:24 | R-0 | R-0 | R-0 | R-0 | R-0 | R-0 | R-0 | R-0 |
| RXOVF31 | RXOVF30 | RXOVF29 | RXOVF28 | RXOVF27 | RXOVF26 | RXOVF25 | RXOVF24 |
| 23:16 | R-0 | R-0 | R-0 | R-0 | R-0 | R-0 | R-0 | R-0 |
| RXOVF23 | RXOVF22 | RXOVF21 | RXOVF20 | RXOVF19 | RXOVF18 | RXOVF17 | RXOVF16 |
| 15:8 | R-0 | R-0 | R-0 | R-0 | R-0 | R-0 | R-0 | R-0 |
| RXOVF15 | RXOVF14 | RXOVF13 | RXOVF12 | RXOVF11 | RXOVF10 | RXOVF9 | RXOVF8 |
| 7:0 | R-0 | R-0 | R-0 | R-0 | R-0 | R-0 | R-0 | R-0 |
| RXOVF7 | RXOVF6 | RXOVF5 | RXOVF4 | RXOVF3 | RXOVF2 | RXOVF1 | RXOVF0 |

| **Legend:** |     |     |
| --- | --- |     | --- |
| R = Readable bit | W = Writable bit | U = Unimplemented bit, read as '0' |
| \-n = Value at POR | '1' = Bit is set | '0' = Bit is cleared x = Bit is unknown |

bit 31-0 **RXOVF&lt;31:0&gt;:** FIFOn Receive Overflow Interrupt Pending bit

1 = FIFO has overflowed

0 = FIFO has not overflowed

### REGISTER 24-8: CiTMR: CAN TIMER REGISTER

| **Bit Range** | **Bit 31/23/15/7** | **Bit 30/22/14/6** | **Bit 29/21/13/5** | **Bit 28/20/12/4** | **Bit 27/19/11/3** | **Bit 26/18/10/2** | **Bit 25/17/9/1** | **Bit 24/16/8/0** |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 31:24 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| CANTS&lt;15:8&gt; |     |     |     |     |     |     |     |
| 23:16 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| CANTS&lt;7:0&gt; |     |     |     |     |     |     |     |
| 15:8 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| CANTSPRE&lt;15:8&gt; |     |     |     |     |     |     |     |
| 7:0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| CANTSPRE&lt;7:0&gt; |     |     |     |     |     |     |     |

**Legend:**

R = Readable bit

\-n = Value at POR

W = Writable bit

'1' = Bit is set

U = Unimplemented bit, read as '0'

'0' = Bit is cleared

x = Bit is unknown

bit 31-0 **CANTS&lt;15:0&gt;:** CAN Time Stamp Timer bits

This is a free-running timer that increments every CANTSPRE system clocks when the CANCAP bit (CiCON&lt;20&gt;) is set.

bit 15-0 **CANTSPRE&lt;15:0&gt;:** CAN Time Stamp Timer Prescaler bits

1111 1111 1111 1111 = CAN time stamp timer (CANTS) increments every 65,535 system clocks

•

•

•

0000 0000 0000 0000 = CAN time stamp timer (CANTS) increments every system clock

**Note 1:** CiTMR will be paused when CANCAP = 0.

**2:** The CiTMR prescaler count will be reset on any write to CiTMR (CANTSPRE will be unaffected).

**REGISTER 24-9: CiRXMn: CAN ACCEPTANCE FILTER MASK 'n' REGISTER (n = 0, 1, 2 OR 3)**

| **Bit Range** | **Bit 31/23/15/7** | **Bit 30/22/14/6** | **Bit 29/21/13/5** | **Bit 28/20/12/4** | **Bit 27/19/11/3** | **Bit 26/18/10/2** | **Bit 25/17/9/1** | **Bit 24/16/8/0** |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 31:24 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| SID&lt;10:3&gt; |     |     |     |     |     |     |     |
| 23:16 | R/W-0 | R/W-0 | R/W-0 | U-0 | R/W-0 | U-0 | R/W-0 | R/W-0 |
| SID&lt;2:0&gt; |     |     | -   | MIDE | -   | EID&lt;17:16&gt; |     |
| 15:8 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| EID&lt;15:8&gt; |     |     |     |     |     |     |     |
| 7:0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| EID&lt;7:0&gt; |     |     |     |     |     |     |     |

| **Legend:** |     |     |
| --- | --- |     | --- |
| R = Readable bit | W = Writable bit | U = Unimplemented bit, read as '0' |
| \-n = Value at POR | '1' = Bit is set | '0' = Bit is cleared x = Bit is unknown |

bit 31-21 **SID&lt;10:0&gt;:** Standard Identifier bits

1 = Include the SIDx bit in filter comparison

0 = The SIDx bit is a 'don't care' in filter operation bit 20 **Unimplemented:** Read as '0'

bit 19 **MIDE:** Identifier Receive Mode bit

1 = Match only message types (standard/extended address) that correspond to the EXID bit in filter

0 = Match either standard or extended address message if filters match (that is, if (Filter SID) = (Message SID) or if (FILTER SID/EID) = (Message SID/EID))

bit 18 **Unimplemented:** Read as '0'

bit 17-0 **EID&lt;17:0&gt;:** Extended Identifier bits

1 = Include the EIDx bit in filter comparison

0 = The EIDx bit is a 'don't care' in filter operation

**Note:** This register can only be modified when the CAN module is in Configuration mode (OPMOD&lt;2:0&gt; (CiCON&lt;23:21&gt;) = 100).

### REGISTER 24-10: CiFLTCON0: CAN FILTER CONTROL REGISTER 0

| **Bit Range** | **Bit 31/23/15/7** | **Bit 30/22/14/6** | **Bit 29/21/13/5** | **Bit 28/20/12/4** | **Bit 27/19/11/3** | **Bit 26/18/10/2** | **Bit 25/17/9/1** | **Bit 24/16/8/0** |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 31:24 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| FLTEN3 | MSEL3&lt;1:0&gt; |     | FSEL3&lt;4:0&gt; |     |     |     |     |
| 23:16 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| FLTEN2 | MSEL2&lt;1:0&gt; |     | FSEL2&lt;4:0&gt; |     |     |     |     |
| 15:8 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| FLTEN1 | MSEL1&lt;1:0&gt; |     | FSEL1&lt;4:0&gt; |     |     |     |     |
| 7:0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| FLTEN0 | MSEL0&lt;1:0&gt; |     | FSEL0&lt;4:0&gt; |     |     |     |     |

| **Legend:** |     |     |
| --- | --- |     | --- |
| R = Readable bit | W = Writable bit | U = Unimplemented bit, read as '0' |
| \-n = Value at POR | '1' = Bit is set | '0' = Bit is cleared x = Bit is unknown |

bit 31 **FLTEN3:** Filter 3 Enable bit

1 = Filter is enabled

0 = Filter is disabled

bit 30-29 **MSEL3&lt;1:0&gt;:** Filter 3 Mask Select bits

11 = Acceptance Mask 3 selected

10 = Acceptance Mask 2 selected

01 = Acceptance Mask 1 selected

00 = Acceptance Mask 0 selected bit 28-24 **FSEL3&lt;4:0&gt;:** FIFO Selection bits

11111 = Message matching filter is stored in FIFO buffer 31

11110 = Message matching filter is stored in FIFO buffer 30

•

•

•

00001 = Message matching filter is stored in FIFO buffer 1

00000 = Message matching filter is stored in FIFO buffer 0 bit 23 **FLTEN2:** Filter 2 Enable bit

1 = Filter is enabled

0 = Filter is disabled

bit 22-21 **MSEL2&lt;1:0&gt;:** Filter 2 Mask Select bits

11 = Acceptance Mask 3 selected

10 = Acceptance Mask 2 selected

01 = Acceptance Mask 1 selected

00 = Acceptance Mask 0 selected bit 20-16 **FSEL2&lt;4:0&gt;:** FIFO Selection bits

11111 = Message matching filter is stored in FIFO buffer 31

11110 = Message matching filter is stored in FIFO buffer 30

•

•

•

00001 = Message matching filter is stored in FIFO buffer 1

00000 = Message matching filter is stored in FIFO buffer 0

**Note:** The bits in this register can only be modified if the corresponding filter enable (FLTENn) bit is '0'.

### REGISTER 24-10: CiFLTCON0: CAN FILTER CONTROL REGISTER 0 (CONTINUED)

bit 15 **FLTEN1:** Filter 1 Enable bit

1 = Filter is enabled

0 = Filter is disabled

bit 14-13 **MSEL1&lt;1:0&gt;:** Filter 1 Mask Select bits

11 = Acceptance Mask 3 selected

10 = Acceptance Mask 2 selected

01 = Acceptance Mask 1 selected

00 = Acceptance Mask 0 selected bit 12-8 **FSEL1&lt;4:0&gt;:** FIFO Selection bits

11111 = Message matching filter is stored in FIFO buffer 31

11110 = Message matching filter is stored in FIFO buffer 30

•

•

•

00001 = Message matching filter is stored in FIFO buffer 1

00000 = Message matching filter is stored in FIFO buffer 0 bit 7 **FLTEN0:** Filter 0 Enable bit

1 = Filter is enabled

0 = Filter is disabled

bit 6-5 **MSEL0&lt;1:0&gt;:** Filter 0 Mask Select bits

11 = Acceptance Mask 3 selected

10 = Acceptance Mask 2 selected

01 = Acceptance Mask 1 selected

00 = Acceptance Mask 0 selected bit 4-0 **FSEL0&lt;4:0&gt;:** FIFO Selection bits

11111 = Message matching filter is stored in FIFO buffer 31

11110 = Message matching filter is stored in FIFO buffer 30

•

•

•

00001 = Message matching filter is stored in FIFO buffer 1

00000 = Message matching filter is stored in FIFO buffer 0

**Note:** The bits in this register can only be modified if the corresponding filter enable (FLTENn) bit is '0'.

### REGISTER 24-11: CiFLTCON1: CAN FILTER CONTROL REGISTER 1

| **Bit Range** | **Bit 31/23/15/7** | **Bit 30/22/14/6** | **Bit 29/21/13/5** | **Bit 28/20/12/4** | **Bit 27/19/11/3** | **Bit 26/18/10/2** | **Bit 25/17/9/1** | **Bit 24/16/8/0** |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 31:24 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| FLTEN7 | MSEL7&lt;1:0&gt; |     | FSEL7&lt;4:0&gt; |     |     |     |     |
| 23:16 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| FLTEN6 | MSEL6&lt;1:0&gt; |     | FSEL6&lt;4:0&gt; |     |     |     |     |
| 15:8 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| FLTEN5 | MSEL5&lt;1:0&gt; |     | FSEL5&lt;4:0&gt; |     |     |     |     |
| 7:0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| FLTEN4 | MSEL4&lt;1:0&gt; |     | FSEL4&lt;4:0&gt; |     |     |     |     |

| **Legend:** |     |     |
| --- | --- |     | --- |
| R = Readable bit | W = Writable bit | U = Unimplemented bit, read as '0' |
| \-n = Value at POR | '1' = Bit is set | '0' = Bit is cleared x = Bit is unknown |

bit 31 **FLTEN7:** Filter 7 Enable bit

1 = Filter is enabled

0 = Filter is disabled

bit 30-29 **MSEL7&lt;1:0&gt;:** Filter 7 Mask Select bits

11 = Acceptance Mask 3 selected

10 = Acceptance Mask 2 selected

01 = Acceptance Mask 1 selected

00 = Acceptance Mask 0 selected bit 28-24 **FSEL7&lt;4:0&gt;:** FIFO Selection bits

11111 = Message matching filter is stored in FIFO buffer 31

11110 = Message matching filter is stored in FIFO buffer 30

•

•

•

00001 = Message matching filter is stored in FIFO buffer 1

00000 = Message matching filter is stored in FIFO buffer 0 bit 23 **FLTEN6:** Filter 6 Enable bit

1 = Filter is enabled

0 = Filter is disabled

bit 22-21 **MSEL6&lt;1:0&gt;:** Filter 6 Mask Select bits

11 = Acceptance Mask 3 selected

10 = Acceptance Mask 2 selected

01 = Acceptance Mask 1 selected

00 = Acceptance Mask 0 selected bit 20-16 **FSEL6&lt;4:0&gt;:** FIFO Selection bits

11111 = Message matching filter is stored in FIFO buffer 31

11110 = Message matching filter is stored in FIFO buffer 30

•

•

•

00001 = Message matching filter is stored in FIFO buffer 1

00000 = Message matching filter is stored in FIFO buffer 0

**Note:** The bits in this register can only be modified if the corresponding filter enable (FLTENn) bit is '0'.

### REGISTER 24-11: CiFLTCON1: CAN FILTER CONTROL REGISTER 1 (CONTINUED)

bit 15 **FLTEN5:** Filter 17 Enable bit

1 = Filter is enabled

0 = Filter is disabled

bit 14-13 **MSEL5&lt;1:0&gt;:** Filter 5 Mask Select bits

11 = Acceptance Mask 3 selected

10 = Acceptance Mask 2 selected

01 = Acceptance Mask 1 selected

00 = Acceptance Mask 0 selected bit 12-8 **FSEL5&lt;4:0&gt;:** FIFO Selection bits

11111 = Message matching filter is stored in FIFO buffer 31

11110 = Message matching filter is stored in FIFO buffer 30

•

•

•

00001 = Message matching filter is stored in FIFO buffer 1

00000 = Message matching filter is stored in FIFO buffer 0 bit 7 **FLTEN4:** Filter 4 Enable bit

1 = Filter is enabled

0 = Filter is disabled

bit 6-5 **MSEL4&lt;1:0&gt;:** Filter 4 Mask Select bits

11 = Acceptance Mask 3 selected

10 = Acceptance Mask 2 selected

01 = Acceptance Mask 1 selected

00 = Acceptance Mask 0 selected bit 4-0 **FSEL4&lt;4:0&gt;:** FIFO Selection bits

11111 = Message matching filter is stored in FIFO buffer 31

11110 = Message matching filter is stored in FIFO buffer 30

•

•

•

00001 = Message matching filter is stored in FIFO buffer 1

00000 = Message matching filter is stored in FIFO buffer 0

**Note:** The bits in this register can only be modified if the corresponding filter enable (FLTENn) bit is '0'.

### REGISTER 24-12: CiFLTCON2: CAN FILTER CONTROL REGISTER 2

| **Bit Range** | **Bit 31/23/15/7** | **Bit 30/22/14/6** | **Bit 29/21/13/5** | **Bit 28/20/12/4** | **Bit 27/19/11/3** | **Bit 26/18/10/2** | **Bit 25/17/9/1** | **Bit 24/16/8/0** |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 31:24 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| FLTEN11 | MSEL11&lt;1:0&gt; |     | FSEL11&lt;4:0&gt; |     |     |     |     |
| 23:16 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| FLTEN10 | MSEL10&lt;1:0&gt; |     | FSEL10&lt;4:0&gt; |     |     |     |     |
| 15:8 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| FLTEN9 | MSEL9&lt;1:0&gt; |     | FSEL9&lt;4:0&gt; |     |     |     |     |
| 7:0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| FLTEN8 | MSEL8&lt;1:0&gt; |     | FSEL8&lt;4:0&gt; |     |     |     |     |

| **Legend:** |     |     |
| --- | --- |     | --- |
| R = Readable bit | W = Writable bit | U = Unimplemented bit, read as '0' |
| \-n = Value at POR | '1' = Bit is set | '0' = Bit is cleared x = Bit is unknown |

bit 31 **FLTEN11:** Filter 11 Enable bit

1 = Filter is enabled

0 = Filter is disabled

bit 30-29 **MSEL11&lt;1:0&gt;:** Filter 11 Mask Select bits

11 = Acceptance Mask 3 selected

10 = Acceptance Mask 2 selected

01 = Acceptance Mask 1 selected

00 = Acceptance Mask 0 selected bit 28-24 **FSEL11&lt;4:0&gt;:** FIFO Selection bits

11111 = Message matching filter is stored in FIFO buffer 31

11110 = Message matching filter is stored in FIFO buffer 30

•

•

•

00001 = Message matching filter is stored in FIFO buffer 1

00000 = Message matching filter is stored in FIFO buffer 0 bit 23 **FLTEN10:** Filter 10 Enable bit

1 = Filter is enabled

0 = Filter is disabled

bit 22-21 **MSEL10&lt;1:0&gt;:** Filter 10 Mask Select bits

11 = Acceptance Mask 3 selected

10 = Acceptance Mask 2 selected

01 = Acceptance Mask 1 selected

00 = Acceptance Mask 0 selected bit 20-16 **FSEL10&lt;4:0&gt;:** FIFO Selection bits

11111 = Message matching filter is stored in FIFO buffer 31

11110 = Message matching filter is stored in FIFO buffer 30

•

•

•

00001 = Message matching filter is stored in FIFO buffer 1

00000 = Message matching filter is stored in FIFO buffer 0

**Note:** The bits in this register can only be modified if the corresponding filter enable (FLTENn) bit is '0'.

### REGISTER 24-12: CiFLTCON2: CAN FILTER CONTROL REGISTER 2 (CONTINUED)

bit 15 **FLTEN9:** Filter 9 Enable bit

1 = Filter is enabled

0 = Filter is disabled

bit 14-13 **MSEL9&lt;1:0&gt;:** Filter 9 Mask Select bits

11 = Acceptance Mask 3 selected

10 = Acceptance Mask 2 selected

01 = Acceptance Mask 1 selected

00 = Acceptance Mask 0 selected bit 12-8 **FSEL9&lt;4:0&gt;:** FIFO Selection bits

11111 = Message matching filter is stored in FIFO buffer 31

11110 = Message matching filter is stored in FIFO buffer 30

•

•

•

00001 = Message matching filter is stored in FIFO buffer 1

00000 = Message matching filter is stored in FIFO buffer 0 bit 7 **FLTEN8:** Filter 8 Enable bit

1 = Filter is enabled

0 = Filter is disabled

bit 6-5 **MSEL8&lt;1:0&gt;:** Filter 8 Mask Select bits

11 = Acceptance Mask 3 selected

10 = Acceptance Mask 2 selected

01 = Acceptance Mask 1 selected

00 = Acceptance Mask 0 selected bit 4-0 **FSEL8&lt;4:0&gt;:** FIFO Selection bits

11111 = Message matching filter is stored in FIFO buffer 31

11110 = Message matching filter is stored in FIFO buffer 30

•

•

•

00001 = Message matching filter is stored in FIFO buffer 1

00000 = Message matching filter is stored in FIFO buffer 0

**Note:** The bits in this register can only be modified if the corresponding filter enable (FLTENn) bit is '0'.

### REGISTER 24-13: CiFLTCON3: CAN FILTER CONTROL REGISTER 3

| **Bit Range** | **Bit 31/23/15/7** | **Bit 30/22/14/6** | **Bit 29/21/13/5** | **Bit 28/20/12/4** | **Bit 27/19/11/3** | **Bit 26/18/10/2** | **Bit 25/17/9/1** | **Bit 24/16/8/0** |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 31:24 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| FLTEN15 | MSEL15&lt;1:0&gt; |     | FSEL15&lt;4:0&gt; |     |     |     |     |
| 23:16 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| FLTEN14 | MSEL14&lt;1:0&gt; |     | FSEL14&lt;4:0&gt; |     |     |     |     |
| 15:8 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| FLTEN13 | MSEL13&lt;1:0&gt; |     | FSEL13&lt;4:0&gt; |     |     |     |     |
| 7:0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| FLTEN12 | MSEL12&lt;1:0&gt; |     | FSEL12&lt;4:0&gt; |     |     |     |     |

| **Legend:** |     |     |
| --- | --- |     | --- |
| R = Readable bit | W = Writable bit | U = Unimplemented bit, read as '0' |
| \-n = Value at POR | '1' = Bit is set | '0' = Bit is cleared x = Bit is unknown |

bit 31 **FLTEN15:** Filter 15 Enable bit

1 = Filter is enabled

0 = Filter is disabled

bit 30-29 **MSEL15&lt;1:0&gt;:** Filter 15 Mask Select bits

11 = Acceptance Mask 3 selected

10 = Acceptance Mask 2 selected

01 = Acceptance Mask 1 selected

00 = Acceptance Mask 0 selected bit 28-24 **FSEL15&lt;4:0&gt;:** FIFO Selection bits

11111 = Message matching filter is stored in FIFO buffer 31

11110 = Message matching filter is stored in FIFO buffer 30

•

•

•

00001 = Message matching filter is stored in FIFO buffer 1

00000 = Message matching filter is stored in FIFO buffer 0 bit 23 **FLTEN14:** Filter 14 Enable bit

1 = Filter is enabled

0 = Filter is disabled

bit 22-21 **MSEL14&lt;1:0&gt;:** Filter 14 Mask Select bits

11 = Acceptance Mask 3 selected

10 = Acceptance Mask 2 selected

01 = Acceptance Mask 1 selected

00 = Acceptance Mask 0 selected bit 20-16 **FSEL14&lt;4:0&gt;:** FIFO Selection bits

11111 = Message matching filter is stored in FIFO buffer 31

11110 = Message matching filter is stored in FIFO buffer 30

•

•

•

00001 = Message matching filter is stored in FIFO buffer 1

00000 = Message matching filter is stored in FIFO buffer 0

**Note:** The bits in this register can only be modified if the corresponding filter enable (FLTENn) bit is '0'.

### REGISTER 24-13: CiFLTCON3: CAN FILTER CONTROL REGISTER 3 (CONTINUED)

bit 15 **FLTEN13:** Filter 13 Enable bit

1 = Filter is enabled

0 = Filter is disabled

bit 14-13 **MSEL13&lt;1:0&gt;:** Filter 13 Mask Select bits

11 = Acceptance Mask 3 selected

10 = Acceptance Mask 2 selected

01 = Acceptance Mask 1 selected

00 = Acceptance Mask 0 selected bit 12-8 **FSEL13&lt;4:0&gt;:** FIFO Selection bits

11111 = Message matching filter is stored in FIFO buffer 31

11110 = Message matching filter is stored in FIFO buffer 30

•

•

•

00001 = Message matching filter is stored in FIFO buffer 1

00000 = Message matching filter is stored in FIFO buffer 0 bit 7 **FLTEN12:** Filter 12 Enable bit

1 = Filter is enabled

0 = Filter is disabled

bit 6-5 **MSEL12&lt;1:0&gt;:** Filter 12 Mask Select bits

11 = Acceptance Mask 3 selected

10 = Acceptance Mask 2 selected

01 = Acceptance Mask 1 selected

00 = Acceptance Mask 0 selected bit 4-0 **FSEL12&lt;4:0&gt;:** FIFO Selection bits

11111 = Message matching filter is stored in FIFO buffer 31

11110 = Message matching filter is stored in FIFO buffer 30

•

•

•

00001 = Message matching filter is stored in FIFO buffer 1

00000 = Message matching filter is stored in FIFO buffer 0

**Note:** The bits in this register can only be modified if the corresponding filter enable (FLTENn) bit is '0'.

,4

### REGISTER 24-14: CiFLTCON4: CAN FILTER CONTROL REGISTER 4

| **Bit Range** | **Bit 31/23/15/7** | **Bit 30/22/14/6** | **Bit 29/21/13/5** | **Bit 28/20/12/4** | **Bit 27/19/11/3** | **Bit 26/18/10/2** | **Bit 25/17/9/1** | **Bit 24/16/8/0** |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 31:24 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| FLTEN19 | MSEL19&lt;1:0&gt; |     | FSEL19&lt;4:0&gt; |     |     |     |     |
| 23:16 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| FLTEN18 | MSEL18&lt;1:0&gt; |     | FSEL18&lt;4:0&gt; |     |     |     |     |
| 15:8 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| FLTEN17 | MSEL17&lt;1:0&gt; |     | FSEL17&lt;4:0&gt; |     |     |     |     |
| 7:0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| FLTEN16 | MSEL16&lt;1:0&gt; |     | FSEL16&lt;4:0&gt; |     |     |     |     |

| **Legend:** |     |     |
| --- | --- |     | --- |
| R = Readable bit | W = Writable bit | U = Unimplemented bit, read as '0' |
| \-n = Value at POR | '1' = Bit is set | '0' = Bit is cleared x = Bit is unknown |

bit 31 **FLTEN19:** Filter 19 Enable bit

1 = Filter is enabled

0 = Filter is disabled

bit 30-29 **MSEL19&lt;1:0&gt;:** Filter 19 Mask Select bits

11 = Acceptance Mask 3 selected

10 = Acceptance Mask 2 selected

01 = Acceptance Mask 1 selected

00 = Acceptance Mask 0 selected bit 28-24 **FSEL19&lt;4:0&gt;:** FIFO Selection bits

11111 = Message matching filter is stored in FIFO buffer 31

11110 = Message matching filter is stored in FIFO buffer 30

•

•

•

00001 = Message matching filter is stored in FIFO buffer 1

00000 = Message matching filter is stored in FIFO buffer 0 bit 23 **FLTEN18:** Filter 18 Enable bit

1 = Filter is enabled

0 = Filter is disabled

bit 22-21 **MSEL18&lt;1:0&gt;:** Filter 18 Mask Select bits

11 = Acceptance Mask 3 selected

10 = Acceptance Mask 2 selected

01 = Acceptance Mask 1 selected

00 = Acceptance Mask 0 selected bit 20-16 **FSEL18&lt;4:0&gt;:** FIFO Selection bits

11111 = Message matching filter is stored in FIFO buffer 31

11110 = Message matching filter is stored in FIFO buffer 30

•

•

•

00001 = Message matching filter is stored in FIFO buffer 1

00000 = Message matching filter is stored in FIFO buffer 0

**Note:** The bits in this register can only be modified if the corresponding filter enable (FLTENn) bit is '0'.

### REGISTER 24-14: CiFLTCON4: CAN FILTER CONTROL REGISTER 4 (CONTINUED)

bit 15 **FLTEN17:** Filter 13 Enable bit

1 = Filter is enabled

0 = Filter is disabled

bit 14-13 **MSEL17&lt;1:0&gt;:** Filter 17 Mask Select bits

11 = Acceptance Mask 3 selected

10 = Acceptance Mask 2 selected

01 = Acceptance Mask 1 selected

00 = Acceptance Mask 0 selected bit 12-8 **FSEL17&lt;4:0&gt;:** FIFO Selection bits

11111 = Message matching filter is stored in FIFO buffer 31

11110 = Message matching filter is stored in FIFO buffer 30

•

•

•

00001 = Message matching filter is stored in FIFO buffer 1

00000 = Message matching filter is stored in FIFO buffer 0 bit 7 **FLTEN16:** Filter 16 Enable bit

1 = Filter is enabled

0 = Filter is disabled

bit 6-5 **MSEL16&lt;1:0&gt;:** Filter 16 Mask Select bits

11 = Acceptance Mask 3 selected

10 = Acceptance Mask 2 selected

01 = Acceptance Mask 1 selected

00 = Acceptance Mask 0 selected bit 4-0 **FSEL16&lt;4:0&gt;:** FIFO Selection bits

11111 = Message matching filter is stored in FIFO buffer 31

11110 = Message matching filter is stored in FIFO buffer 30

•

•

•

00001 = Message matching filter is stored in FIFO buffer 1

00000 = Message matching filter is stored in FIFO buffer 0

**Note:** The bits in this register can only be modified if the corresponding filter enable (FLTENn) bit is '0'.

### REGISTER 24-15: CiFLTCON5: CAN FILTER CONTROL REGISTER 5

| **Bit Range** | **Bit 31/23/15/7** | **Bit 30/22/14/6** | **Bit 29/21/13/5** | **Bit 28/20/12/4** | **Bit 27/19/11/3** | **Bit 26/18/10/2** | **Bit 25/17/9/1** | **Bit 24/16/8/0** |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 31:24 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| FLTEN23 | MSEL23&lt;1:0&gt; |     | FSEL23&lt;4:0&gt; |     |     |     |     |
| 23:16 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| FLTEN22 | MSEL22&lt;1:0&gt; |     | FSEL22&lt;4:0&gt; |     |     |     |     |
| 15:8 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| FLTEN21 | MSEL21&lt;1:0&gt; |     | FSEL21&lt;4:0&gt; |     |     |     |     |
| 7:0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| FLTEN20 | MSEL20&lt;1:0&gt; |     | FSEL20&lt;4:0&gt; |     |     |     |     |

| **Legend:** |     |     |
| --- | --- |     | --- |
| R = Readable bit | W = Writable bit | U = Unimplemented bit, read as '0' |
| \-n = Value at POR | '1' = Bit is set | '0' = Bit is cleared x = Bit is unknown |

bit 31 **FLTEN23:** Filter 23 Enable bit

1 = Filter is enabled

0 = Filter is disabled

bit 30-29 **MSEL23&lt;1:0&gt;:** Filter 23 Mask Select bits

11 = Acceptance Mask 3 selected

10 = Acceptance Mask 2 selected

01 = Acceptance Mask 1 selected

00 = Acceptance Mask 0 selected bit 28-24 **FSEL23&lt;4:0&gt;:** FIFO Selection bits

11111 = Message matching filter is stored in FIFO buffer 31

11110 = Message matching filter is stored in FIFO buffer 30

•

•

•

00001 = Message matching filter is stored in FIFO buffer 1

00000 = Message matching filter is stored in FIFO buffer 0 bit 23 **FLTEN22:** Filter 22 Enable bit

1 = Filter is enabled

0 = Filter is disabled

bit 22-21 **MSEL22&lt;1:0&gt;:** Filter 22 Mask Select bits

11 = Acceptance Mask 3 selected

10 = Acceptance Mask 2 selected

01 = Acceptance Mask 1 selected

00 = Acceptance Mask 0 selected bit 20-16 **FSEL22&lt;4:0&gt;:** FIFO Selection bits

11111 = Message matching filter is stored in FIFO buffer 31

11110 = Message matching filter is stored in FIFO buffer 30

•

•

•

00001 = Message matching filter is stored in FIFO buffer 1

00000 = Message matching filter is stored in FIFO buffer 0

**Note:** The bits in this register can only be modified if the corresponding filter enable (FLTENn) bit is '0'.

### REGISTER 24-15: CiFLTCON5: CAN FILTER CONTROL REGISTER 5 (CONTINUED)

bit 15 **FLTEN21:** Filter 21 Enable bit

1 = Filter is enabled

0 = Filter is disabled

bit 14-13 **MSEL21&lt;1:0&gt;:** Filter 21 Mask Select bits

11 = Acceptance Mask 3 selected

10 = Acceptance Mask 2 selected

01 = Acceptance Mask 1 selected

00 = Acceptance Mask 0 selected bit 12-8 **FSEL21&lt;4:0&gt;:** FIFO Selection bits

11111 = Message matching filter is stored in FIFO buffer 31

11110 = Message matching filter is stored in FIFO buffer 30

•

•

•

00001 = Message matching filter is stored in FIFO buffer 1

00000 = Message matching filter is stored in FIFO buffer 0 bit 7 **FLTEN20:** Filter 20 Enable bit

1 = Filter is enabled

0 = Filter is disabled

bit 6-5 **MSEL20&lt;1:0&gt;:** Filter 20 Mask Select bits

11 = Acceptance Mask 3 selected

10 = Acceptance Mask 2 selected

01 = Acceptance Mask 1 selected

00 = Acceptance Mask 0 selected bit 4-0 **FSEL20&lt;4:0&gt;:** FIFO Selection bits

11111 = Message matching filter is stored in FIFO buffer 31

11110 = Message matching filter is stored in FIFO buffer 30

•

•

•

00001 = Message matching filter is stored in FIFO buffer 1

00000 = Message matching filter is stored in FIFO buffer 0

**Note:** The bits in this register can only be modified if the corresponding filter enable (FLTENn) bit is '0'.

### REGISTER 24-16: CiFLTCON6: CAN FILTER CONTROL REGISTER 6

| **Bit Range** | **Bit 31/23/15/7** | **Bit 30/22/14/6** | **Bit 29/21/13/5** | **Bit 28/20/12/4** | **Bit 27/19/11/3** | **Bit 26/18/10/2** | **Bit 25/17/9/1** | **Bit 24/16/8/0** |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 31:24 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| FLTEN27 | MSEL27&lt;1:0&gt; |     | FSEL27&lt;4:0&gt; |     |     |     |     |
| 23:16 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| FLTEN26 | MSEL26&lt;1:0&gt; |     | FSEL26&lt;4:0&gt; |     |     |     |     |
| 15:8 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| FLTEN25 | MSEL25&lt;1:0&gt; |     | FSEL25&lt;4:0&gt; |     |     |     |     |
| 7:0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| FLTEN24 | MSEL24&lt;1:0&gt; |     | FSEL24&lt;4:0&gt; |     |     |     |     |

| **Legend:** |     |     |
| --- | --- |     | --- |
| R = Readable bit | W = Writable bit | U = Unimplemented bit, read as '0' |
| \-n = Value at POR | '1' = Bit is set | '0' = Bit is cleared x = Bit is unknown |

bit 31 **FLTEN27:** Filter 27 Enable bit

1 = Filter is enabled

0 = Filter is disabled

bit 30-29 **MSEL27&lt;1:0&gt;:** Filter 27 Mask Select bits

11 = Acceptance Mask 3 selected

10 = Acceptance Mask 2 selected

01 = Acceptance Mask 1 selected

00 = Acceptance Mask 0 selected bit 28-24 **FSEL27&lt;4:0&gt;:** FIFO Selection bits

11111 = Message matching filter is stored in FIFO buffer 31

11110 = Message matching filter is stored in FIFO buffer 30

•

•

•

00001 = Message matching filter is stored in FIFO buffer 1

00000 = Message matching filter is stored in FIFO buffer 0 bit 23 **FLTEN26:** Filter 26 Enable bit

1 = Filter is enabled

0 = Filter is disabled

bit 22-21 **MSEL26&lt;1:0&gt;:** Filter 26 Mask Select bits

11 = Acceptance Mask 3 selected

10 = Acceptance Mask 2 selected

01 = Acceptance Mask 1 selected

00 = Acceptance Mask 0 selected bit 20-16 **FSEL26&lt;4:0&gt;:** FIFO Selection bits

11111 = Message matching filter is stored in FIFO buffer 31

11110 = Message matching filter is stored in FIFO buffer 30

•

•

•

00001 = Message matching filter is stored in FIFO buffer 1

00000 = Message matching filter is stored in FIFO buffer 0

**Note:** The bits in this register can only be modified if the corresponding filter enable (FLTENn) bit is '0'.

### REGISTER 24-16: CiFLTCON6: CAN FILTER CONTROL REGISTER 6 (CONTINUED)

bit 15 **FLTEN25:** Filter 25 Enable bit

1 = Filter is enabled

0 = Filter is disabled

bit 14-13 **MSEL25&lt;1:0&gt;:** Filter 25 Mask Select bits

11 = Acceptance Mask 3 selected

10 = Acceptance Mask 2 selected

01 = Acceptance Mask 1 selected

00 = Acceptance Mask 0 selected bit 12-8 **FSEL25&lt;4:0&gt;:** FIFO Selection bits

11111 = Message matching filter is stored in FIFO buffer 31

11110 = Message matching filter is stored in FIFO buffer 30

•

•

•

00001 = Message matching filter is stored in FIFO buffer 1

00000 = Message matching filter is stored in FIFO buffer 0 bit 7 **FLTEN24:** Filter 24 Enable bit

1 = Filter is enabled

0 = Filter is disabled

bit 6-5 **MSEL24&lt;1:0&gt;:** Filter 24 Mask Select bits

11 = Acceptance Mask 3 selected

10 = Acceptance Mask 2 selected

01 = Acceptance Mask 1 selected

00 = Acceptance Mask 0 selected bit 4-0 **FSEL24&lt;4:0&gt;:** FIFO Selection bits

11111 = Message matching filter is stored in FIFO buffer 31

11110 = Message matching filter is stored in FIFO buffer 30

•

•

•

00001 = Message matching filter is stored in FIFO buffer 1

00000 = Message matching filter is stored in FIFO buffer 0

**Note:** The bits in this register can only be modified if the corresponding filter enable (FLTENn) bit is '0'.

### REGISTER 24-17: CiFLTCON7: CAN FILTER CONTROL REGISTER 7

| **Bit Range** | **Bit 31/23/15/7** | **Bit 30/22/14/6** | **Bit 29/21/13/5** | **Bit 28/20/12/4** | **Bit 27/19/11/3** | **Bit 26/18/10/2** | **Bit 25/17/9/1** | **Bit 24/16/8/0** |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 31:24 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| FLTEN31 | MSEL31&lt;1:0&gt; |     | FSEL31&lt;4:0&gt; |     |     |     |     |
| 23:16 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| FLTEN30 | MSEL30&lt;1:0&gt; |     | FSEL30&lt;4:0&gt; |     |     |     |     |
| 15:8 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| FLTEN29 | MSEL29&lt;1:0&gt; |     | FSEL29&lt;4:0&gt; |     |     |     |     |
| 7:0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| FLTEN28 | MSEL28&lt;1:0&gt; |     | FSEL28&lt;4:0&gt; |     |     |     |     |

| **Legend:** |     |     |
| --- | --- |     | --- |
| R = Readable bit | W = Writable bit | U = Unimplemented bit, read as '0' |
| \-n = Value at POR | '1' = Bit is set | '0' = Bit is cleared x = Bit is unknown |

bit 31 **FLTEN31:** Filter 31 Enable bit

1 = Filter is enabled

0 = Filter is disabled

bit 30-29 **MSEL31&lt;1:0&gt;:** Filter 31 Mask Select bits

11 = Acceptance Mask 3 selected

10 = Acceptance Mask 2 selected

01 = Acceptance Mask 1 selected

00 = Acceptance Mask 0 selected bit 28-24 **FSEL31&lt;4:0&gt;:** FIFO Selection bits

11111 = Message matching filter is stored in FIFO buffer 31

11110 = Message matching filter is stored in FIFO buffer 30

•

•

•

00001 = Message matching filter is stored in FIFO buffer 1

00000 = Message matching filter is stored in FIFO buffer 0 bit 23 **FLTEN30:** Filter 30Enable bit

1 = Filter is enabled

0 = Filter is disabled

bit 22-21 **MSEL30&lt;1:0&gt;:** Filter 30Mask Select bits

11 = Acceptance Mask 3 selected

10 = Acceptance Mask 2 selected

01 = Acceptance Mask 1 selected

00 = Acceptance Mask 0 selected bit 20-16 **FSEL30&lt;4:0&gt;:** FIFO Selection bits

11111 = Message matching filter is stored in FIFO buffer 31

11110 = Message matching filter is stored in FIFO buffer 30

•

•

•

00001 = Message matching filter is stored in FIFO buffer 1

00000 = Message matching filter is stored in FIFO buffer 0

**Note:** The bits in this register can only be modified if the corresponding filter enable (FLTENn) bit is '0'.

### REGISTER 24-17: CiFLTCON7: CAN FILTER CONTROL REGISTER 7 (CONTINUED)

bit 15 **FLTEN29:** Filter 29 Enable bit

1 = Filter is enabled

0 = Filter is disabled

bit 14-13 **MSEL29&lt;1:0&gt;:** Filter 29 Mask Select bits

11 = Acceptance Mask 3 selected

10 = Acceptance Mask 2 selected

01 = Acceptance Mask 1 selected

00 = Acceptance Mask 0 selected bit 12-8 **FSEL29&lt;4:0&gt;:** FIFO Selection bits

11111 = Message matching filter is stored in FIFO buffer 31

11110 = Message matching filter is stored in FIFO buffer 30

•

•

•

00001 = Message matching filter is stored in FIFO buffer 1

00000 = Message matching filter is stored in FIFO buffer 0 bit 7 **FLTEN28:** Filter 28 Enable bit

1 = Filter is enabled

0 = Filter is disabled

bit 6-5 **MSEL28&lt;1:0&gt;:** Filter 28 Mask Select bits

11 = Acceptance Mask 3 selected

10 = Acceptance Mask 2 selected

01 = Acceptance Mask 1 selected

00 = Acceptance Mask 0 selected bit 4-0 **FSEL28&lt;4:0&gt;:** FIFO Selection bits

11111 = Message matching filter is stored in FIFO buffer 31

11110 = Message matching filter is stored in FIFO buffer 30

•

•

•

00001 = Message matching filter is stored in FIFO buffer 1

00000 = Message matching filter is stored in FIFO buffer 0

**Note:** The bits in this register can only be modified if the corresponding filter enable (FLTENn) bit is '0'.

**REGISTER 24-18: CiRXFn: CAN ACCEPTANCE FILTER 'n' REGISTER 7 (n = 0 THROUGH 31)**

| **Bit Range** | **Bit 31/23/15/7** | **Bit 30/22/14/6** | **Bit 29/21/13/5** | **Bit 28/20/12/4** | **Bit 27/19/11/3** | **Bit 26/18/10/2** | **Bit 25/17/9/1** | **Bit 24/16/8/0** |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 31:24 | R/W-x | R/W-x | R/W-x | R/W-x | R/W-x | R/W-x | R/W-x | R/W-x |
| SID&lt;10:3&gt; |     |     |     |     |     |     |     |
| 23:16 | R/W-x | R/W-x | R/W-x | U-0 | R/W-0 | U-0 | R/W-x | R/W-x |
| SID&lt;2:0&gt; |     |     | -   | EXID | -   | EID&lt;17:16&gt; |     |
| 15:8 | R/W-x | R/W-x | R/W-x | R/W-x | R/W-x | R/W-x | R/W-x | R/W-x |
| EID&lt;15:8&gt; |     |     |     |     |     |     |     |
| 7:0 | R/W-x | R/W-x | R/W-x | R/W-x | R/W-x | R/W-x | R/W-x | R/W-x |
| EID&lt;7:0&gt; |     |     |     |     |     |     |     |

| **Legend:** |     |     |
| --- | --- |     | --- |
| R = Readable bit | W = Writable bit | U = Unimplemented bit, read as '0' |
| \-n = Value at POR | '1' = Bit is set | '0' = Bit is cleared x = Bit is unknown |

bit 31-21 **SID&lt;10:0&gt;:** Standard Identifier bits

1 = Message address bit SIDx must be '1' to match filter

0 = Message address bit SIDx must be '0' to match filter bit 20 **Unimplemented:** Read as '0'

bit 19 **EXID:** Extended Identifier Enable bits

1 = Match only messages with extended identifier addresses

0 = Match only messages with standard identifier addresses bit 18 **Unimplemented:** Read as '0'

bit 17-0 **EID&lt;17:0&gt;:** Extended Identifier bits

1 = Message address bit EIDx must be '1' to match filter

0 = Message address bit EIDx must be '0' to match filter

**Note:** This register can only be modified when the filter is disabled (FLTENn = 0).

### REGISTER 24-19: CiFIFOBA: CAN MESSAGE BUFFER BASE ADDRESS REGISTER

| **Bit Range** | **Bit 31/23/15/7** | **Bit 30/22/14/6** | **Bit 29/21/13/5** | **Bit 28/20/12/4** | **Bit 27/19/11/3** | **Bit 26/18/10/2** | **Bit 25/17/9/1** | **Bit 24/16/8/0** |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 31:24 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| CiFIFOBA&lt;31:24&gt; |     |     |     |     |     |     |     |
| 23:16 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| CiFIFOBA&lt;23:16&gt; |     |     |     |     |     |     |     |
| 15:8 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| CiFIFOBA&lt;15:8&gt; |     |     |     |     |     |     |     |
| 7:0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R-0**(1)** | R-0**(1)** |
| CiFIFOBA&lt;7:0&gt; |     |     |     |     |     |     |     |

| **Legend:** |     |     |
| --- | --- |     | --- |
| R = Readable bit | W = Writable bit | U = Unimplemented bit, read as '0' |
| \-n = Value at POR | '1' = Bit is set | '0' = Bit is cleared x = Bit is unknown |

bit 31-0 **CiFIFOBA&lt;31:0&gt;:** CAN FIFO Base Address bits

These bits define the base address of all message buffers. Individual message buffers are located based on the size of the previous message buffers. This address is a physical address. Bits &lt;1:0&gt; are read-only and read as '0', forcing the messages to be 32-bit word-aligned in device RAM.

**Note 1:** This bit is unimplemented and will always read '0', which forces word-alignment of messages.

**Note:** This register can only be modified when the CAN module is in Configuration mode (OPMOD&lt;2:0&gt; (CiCON&lt;23:21&gt;) = 100).

**REGISTER 24-20: CiFIFOCONn: CAN FIFO CONTROL REGISTER 'n' (n = 0 THROUGH 31)**

| **Bit Range** | **Bit 31/23/15/7** | **Bit 30/22/14/6** | **Bit 29/21/13/5** | **Bit 28/20/12/4** | **Bit 27/19/11/3** | **Bit 26/18/10/2** | **Bit 25/17/9/1** | **Bit 24/16/8/0** |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 31:24 | U-0 | U-0 | U-0 | U-0 | U-0 | U-0 | U-0 | U-0 |
| -   | -   | -   | -   | -   | -   | -   | -   |
| 23:16 | U-0 | U-0 | U-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| -   | -   | -   | FSIZE&lt;4:0&gt;**(1)** |     |     |     |     |
| 15:8 | U-0 | S/HC-0 | S/HC-0 | R/W-0 | U-0 | U-0 | U-0 | U-0 |
| -   | FRESET | UINC | DONLY**(1)** | -   | -   | -   | -   |
| 7:0 | R/W-0 | R-0 | R-0 | R-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| TXEN | TXABAT**(2)** | TXLARB**(3)** | TXERR**(3)** | TXREQ | RTREN | TXPR&lt;1:0&gt; |     |

| **Legend:** |     |     |
| --- | --- |     | --- |
| R = Readable bit | W = Writable bit | U = Unimplemented bit, read as '0' |
| \-n = Value at POR | '1' = Bit is set | '0' = Bit is cleared x = Bit is unknown |

bit 31-21 **Unimplemented:** Read as '0' bit 20-16 **FSIZE&lt;4:0&gt;:** FIFO Size bits**(1)**

11111 = FIFO is 32 messages deep

•

•

•

00010 = FIFO is 3 messages deep 00001 = FIFO is 2 messages deep 00000 = FIFO is 1 message deep

bit 15 **Unimplemented:** Read as '0' bit 14 **FRESET:** FIFO Reset bits

1 = FIFO will be reset when bit is set, cleared by hardware when FIFO is reset. After setting, the user should poll whether this bit is clear before taking any action.

0 = No effect

bit 13 **UINC:** Increment Head/Tail bit

TXEN = 1: (FIFO configured as a Transmit FIFO)

When this bit is set the FIFO head will increment by a single message

TXEN = 0: (FIFO configured as a Receive FIFO)

When this bit is set the FIFO tail will increment by a single message bit 12 **DONLY:** Store Message Data Only bit**(1)**

TXEN = 1: (FIFO configured as a Transmit FIFO) This bit is not used and has no effect.

TXEN = 0: (FIFO configured as a Receive FIFO)

1 = Only data bytes will be stored in the FIFO

0 = Full message is stored, including identifier bit 11-8 **Unimplemented:** Read as '0'

bit 7 **TXEN:** TX/RX Buffer Selection bit

1 = FIFO is a Transmit FIFO

0 = FIFO is a Receive FIFO

**Note 1:** These bits can only be modified when the CAN module is in Configuration mode (OPMOD&lt;2:0&gt; bits (CiCON&lt;23:21&gt;) = 100).

**2:** This bit is updated when a message completes (or aborts) or when the FIFO is reset.

**3:** This bit is reset on any read of this register or when the FIFO is reset.

**REGISTER 24-20: CiFIFOCONn: CAN FIFO CONTROL REGISTER 'n' (n = 0 THROUGH 31)**

bit 6 **TXABAT:** Message Aborted bit**(2)**

1 = Message was aborted

0 = Message completed successfully

bit 5 **TXLARB:** Message Lost Arbitration bit**(3)**

1 = Message lost arbitration while being sent

0 = Message did not lose arbitration while being sent bit 4 **TXERR:** Error Detected During Transmission bit**(3)**

1 = A bus error occured while the message was being sent

0 = A bus error did not occur while the message was being sent bit 3 **TXREQ:** Message Send Request

TXEN = 1: (FIFO configured as a Transmit FIFO) Setting this bit to '1' requests sending a message.

The bit will automatically clear when all the messages queued in the FIFO are successfully sent. Clearing the bit to '0' while set ('1') will request a message abort.

TXEN = 0: (FIFO configured as a receive FIFO) This bit has no effect.

bit 2 **RTREN:** Auto RTR Enable bit

1 = When a remote transmit is received, TXREQ will be set

0 = When a remote transmit is received, TXREQ will be unaffected bit 1-0 **TXPR&lt;1:0&gt;:** Message Transmit Priority bits

11 = Highest message priority

10 = High intermediate message priority 01 = Low intermediate message priority 00 = Lowest message priority

**Note 1:** These bits can only be modified when the CAN module is in Configuration mode (OPMOD&lt;2:0&gt; bits (CiCON&lt;23:21&gt;) = 100).

**2:** This bit is updated when a message completes (or aborts) or when the FIFO is reset.

**3:** This bit is reset on any read of this register or when the FIFO is reset.

**REGISTER 24-21: CiFIFOINTn: CAN FIFO INTERRUPT REGISTER 'n' (n = 0 THROUGH 31)**

| **Bit Range** | **Bit 31/23/15/7** | **Bit 30/22/14/6** | **Bit 29/21/13/5** | **Bit 28/20/12/4** | **Bit 27/19/11/3** | **Bit 26/18/10/2** | **Bit 25/17/9/1** | **Bit 24/16/8/0** |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 31:24 | U-0 | U-0 | U-0 | U-0 | U-0 | R/W-0 | R/W-0 | R/W-0 |
| -   | -   | -   | -   | -   | TXNFULLIE | TXHALFIE | TXEMPTYIE |
| 23:16 | U-0 | U-0 | U-0 | U-0 | R/W-0 | R/W-0 | R/W-0 | R/W-0 |
| -   | -   | -   | -   | RXOVFLIE | RXFULLIE | RXHALFIE | RXNEMPTYIE |
| 15:8 | U-0 | U-0 | U-0 | U-0 | U-0 | R-0 | R-0 | R-0 |
| -   | -   | -   | -   | -   | TXNFULLIF**(1)** | TXHALFIF | TXEMPTYIF**(1)** |
| 7:0 | U-0 | U-0 | U-0 | U-0 | R/W-0 | R-0 | R-0 | R-0 |
| -   | -   | -   | -   | RXOVFLIF | RXFULLIF**(1)** | RXHALFIF**(1)** | RXNEMPTYIF**(1)** |

| **Legend:** |     |     |
| --- | --- |     | --- |
| R = Readable bit | W = Writable bit | U = Unimplemented bit, read as '0' |
| \-n = Value at POR | '1' = Bit is set | '0' = Bit is cleared x = Bit is unknown |

bit 31-27 **Unimplemented:** Read as '0'

bit 26 **TXNFULLIE:** Transmit FIFO Not Full Interrupt Enable bit

1 = Interrupt enabled for FIFO not full

0 = Interrupt disabled for FIFO not full

bit 25 **TXHALFIE:** Transmit FIFO Half Full Interrupt Enable bit

1 = Interrupt enabled for FIFO half full

0 = Interrupt disabled for FIFO half full

bit 24 **TXEMPTYIE:** Transmit FIFO Empty Interrupt Enable bit

1 = Interrupt enabled for FIFO empty

0 = Interrupt disabled for FIFO empty bit 23-20 **Unimplemented:** Read as '0'

bit 19 **RXOVFLIE:** Overflow Interrupt Enable bit

1 = Interrupt enabled for overflow event

0 = Interrupt disabled for overflow event bit 18 **RXFULLIE:** Full Interrupt Enable bit

1 = Interrupt enabled for FIFO full

0 = Interrupt disabled for FIFO full

bit 17 **RXHALFIE:** FIFO Half Full Interrupt Enable bit

1 = Interrupt enabled for FIFO half full

0 = Interrupt disabled for FIFO half full

bit 16 **RXNEMPTYIE:** Empty Interrupt Enable bit

1 = Interrupt enabled for FIFO not empty

0 = Interrupt disabled for FIFO not empty bit 15-11 **Unimplemented:** Read as '0'

bit 10 **TXNFULLIF:** Transmit FIFO Not Full Interrupt Flag bit**(1)**

TXEN = 1: (FIFO configured as a transmit buffer)

1 = FIFO is not full

0 = FIFO is full

TXEN = 0: (FIFO configured as a receive buffer) Unused, reads '0'

**Note 1:** This bit is read-only and reflects the status of the FIFO.

**REGISTER 24-21: CiFIFOINTn: CAN FIFO INTERRUPT REGISTER 'n' (n = 0 THROUGH 31)**

bit 9 **TXHALFIF:** FIFO Transmit FIFO Half Empty Interrupt Flag bit**(1)** TXEN = 1: (FIFO configured as a transmit buffer)

1 = FIFO is  half full

0 = FIFO is > half full

TXEN = 0: (FIFO configured as a receive buffer) Unused, reads '0'

bit 8 **TXEMPTYIF:** Transmit FIFO Empty Interrupt Flag bit**(1)** TXEN = 1: (FIFO configured as a transmit buffer)

1 = FIFO is empty

0 = FIFO is not empty, at least 1 message queued to be transmitted

TXEN = 0: (FIFO configured as a receive buffer) Unused, reads '0'

bit 7-4 **Unimplemented:** Read as '0'

bit 3 **RXOVFLIF:** Receive FIFO Overflow Interrupt Flag bit

TXEN = 1: (FIFO configured as a transmit buffer) Unused, reads '0'

TXEN = 0: (FIFO configured as a receive buffer)

1 = Overflow event has occurred

0 = No overflow event occured

bit 2 **RXFULLIF:** Receive FIFO Full Interrupt Flag bit**(1)**

TXEN = 1: (FIFO configured as a transmit buffer) Unused, reads '0'

TXEN = 0: (FIFO configured as a receive buffer)

1 = FIFO is full

0 = FIFO is not full

bit 1 **RXHALFIF:** Receive FIFO Half Full Interrupt Flag bit**(1)**

TXEN = 1: (FIFO configured as a transmit buffer) Unused, reads '0'

TXEN = 0: (FIFO configured as a receive buffer)

1 = FIFO is  half full

0 = FIFO is < half full

bit 0 **RXNEMPTYIF:** Receive Buffer Not Empty Interrupt Flag bit**(1)** TXEN = 1: (FIFO configured as a transmit buffer)

Unused, reads '0'

TXEN = 0: (FIFO configured as a receive buffer) 1 = FIFO is not empty, has at least 1 message 0 = FIFO is empty

**Note 1:** This bit is read-only and reflects the status of the FIFO.

**REGISTER 24-22: CiFIFOUAn: CAN FIFO USER ADDRESS REGISTER 'n' (n = 0 THROUGH 31)**

| **Bit Range** | **Bit 31/23/15/7** | **Bit 30/22/14/6** | **Bit 29/21/13/5** | **Bit 28/20/12/4** | **Bit 27/19/11/3** | **Bit 26/18/10/2** | **Bit 25/17/9/1** | **Bit 24/16/8/0** |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 31:24 | R-x | R-x | R-x | R-x | R-x | R-x | R-x | R-x |
| CiFIFOUAn&lt;31:24&gt; |     |     |     |     |     |     |     |
| 23:16 | R-x | R-x | R-x | R-x | R-x | R-x | R-x | R-x |
| CiFIFOUAn&lt;23:16&gt; |     |     |     |     |     |     |     |
| 15:8 | R-x | R-x | R-x | R-x | R-x | R-x | R-x | R-x |
| CiFIFOUAn&lt;15:8&gt; |     |     |     |     |     |     |     |
| 7:0 | R-x | R-x | R-x | R-x | R-x | R-x | R-0**(1)** | R-0**(1)** |
| CiFIFOUAn&lt;7:0&gt; |     |     |     |     |     |     |     |

| **Legend:** |     |     |
| --- | --- |     | --- |
| R = Readable bit | W = Writable bit | U = Unimplemented bit, read as '0' |
| \-n = Value at POR | '1' = Bit is set | '0' = Bit is cleared x = Bit is unknown |

bit 31-0 **CiFIFOUAn&lt;31:0&gt;:** CAN FIFO User Address bits TXEN = 1: (FIFO configured as a transmit buffer)

A read of this register will return the address where the next message is to be written (FIFO head).

TXEN = 0: (FIFO configured as a receive buffer)

A read of this register will return the address where the next message is to be read (FIFO tail).

**Note 1:** This bit will always read '0', which forces byte-alignment of messages.

**Note:** This register is not guaranteed to read correctly in Configuration mode, and should only be accessed when the module is not in Configuration mode.

**REGISTER 24-23: CiFIFOCIN: CAN MODULE MESSAGE INDEX REGISTER 'n' (n = 0 THROUGH 31)**

| **Bit Range** | **Bit 31/23/15/7** | **Bit 30/22/14/6** | **Bit 29/21/13/5** | **Bit 28/20/12/4** | **Bit 27/19/11/3** | **Bit 26/18/10/2** | **Bit 25/17/9/1** | **Bit 24/16/8/0** |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 31:24 | U-0 | U-0 | U-0 | U-0 | U-0 | U-0 | U-0 | U-0 |
| -   | -   | -   | -   | -   | -   | -   | -   |
| 23:16 | U-0 | U-0 | U-0 | U-0 | U-0 | U-0 | U-0 | U-0 |
| -   | -   | -   | -   | -   | -   | -   | -   |
| 15:8 | U-0 | U-0 | U-0 | U-0 | U-0 | U-0 | U-0 | U-0 |
| -   | -   | -   | -   | -   | -   | -   | -   |
| 7:0 | U-0 | U-0 | U-0 | R-0 | R-0 | R-0 | R-0 | R-0 |
| -   | -   | -   | CiFIFOCI&lt;4:0&gt; |     |     |     |     |

| **Legend:** |     |     |
| --- | --- |     | --- |
| R = Readable bit | W = Writable bit | U = Unimplemented bit, read as '0' |
| \-n = Value at POR | '1' = Bit is set | '0' = Bit is cleared x = Bit is unknown |

bit 31-5 **Unimplemented:** Read as '0'

bit 4-0 **CiFIFOCIn&lt;4:0&gt;:** CAN Side FIFO Message Index bits

TXEN = 1: (FIFO configured as a transmit buffer)

A read of this register will return an index to the message that the FIFO will next attempt to transmit.

TXEN = 0: (FIFO configured as a receive buffer)

A read of this register will return an index to the message that the FIFO will use to save the next message.