# tag-os
Open Source Laser Tag 

This project is in it's infancy, with loads still to do. The model is based primarily on the Design of Skirmos, a failed kickstarter that was also promised as open-source...


## Key Features
- 3D printable tagger and receivers
- Tagger includes TFT screen, rail for mounting scopes
- On-board sound effects
- Toggle switch for different fire modes

The project consists of the following sections:

## Tagger
This folder contains the STL files for the laser tagger itself. This is mostly 3D printed, but also uses a couple of readily available components (namely PVC pipe and aluminium extrusion; 2020).

The tagger is generally assembled in two halves, and then fixed together once the internal components and electronics are built. If there's not a specific STL for the opposite side, then you can just mirror the STL in your slicer. 

## Electronics
### Overview
Laser taggers generally work using IR LED's and receivers, sending information packets in a similar way to TV remotes. With this project, i'm using a lens to focus that beam to make the guns more accurate and improve the performance over longer distances. 

I'm using an ESP32 micro-controller to handle all the logic and control the various components, which also allows for wifi connectivity and OTA updates. 
This ESP32 (D1 Mini) is mounted on a custom PCB that brings together all the connections needed and makes assembly much easier. Gerber files are available in the `pcb` folder.


## BOM
BOM is a little vague. You'll want to be building a number of taggers etc, and lots of components will come in multi-packs. I've also not included any shipping costs. But this should give you a general idea of cost.

| Item 	| Description 	| Source 	| Price (GBP) 	| Quantity 	| Total 	|
|---	|---	|---	|---	|---	|---	|
| **Hardware** 	|  	|  	|  	|  	|  	|
| 32mm PVC Pipe 	| 36mm OD, 3m 	| [Toolstation](https://www.toolstation.com/solvent-weld-waste-pipe-3m/p65399) 	| 5.58 	| 1 	| 5.58 	|
| Aluminium Extrusion 	| 2020, black 	| [eBay](https://www.ebay.co.uk/itm/204168074414?var=504748872985&hash=item2f895d94ae:g:gjQAAOSwmktfx7B2&amdata=enc%3AAQAIAAAA4KW9bc8WNkdQk4CRbOGc37rQGn1FxpVPqeUE%2Bogyyo5xv3g9tJjZIPAYS3aXIK7H6rEuewGVif0oaT%2BzvEM6cewRyD2oZ78MAOIjSy2H8VwrP%2Byz7xJBIsY%2B8PEZ8HKSEpdS%2BrDLqW0vo1yDa59JPFtM8C%2FrJ%2FeHavM5InIlOKfQn8PB%2F3HBParsAszN9bnkYHsRWGQxTJ8S46d%2F5l2AJqFj2AV0C8Kzr42%2Bxzss3Trm6bFZ8JnRMEG58vJJ2VzkrWLP9TfUnu1mM2R8IpXHBbcyttswo0cRzmy%2BEZF1D39u%7Ctkp%3ABFBMzKKbgLFi) 	| 3.95 	| 1 	| 3.95 	|
| M3 x 8mm bolt 	| pk of 50 	| [Aliexpress](https://www.aliexpress.com/item/32810872544.html?spm=a2g0o.order_list.order_list_main.5.19951802auCtj1) 	| 0.78 	| 1 	| 0.78 	|
| M4 x 8mm bolt 	| pk of 10 	| [Aliexpress](https://www.aliexpress.com/item/1005005070119421.html?spm=a2g0o.order_list.order_list_main.17.19951802auCtj1) 	| 1.90 	| 1 	| 1.90 	|
| M3 T-Nut 	| pk of 30 	| [Aliexpress](https://www.aliexpress.com/item/1005001623335549.html?spm=a2g0o.productlist.main.3.de663ddcPjHEAg&algo_pvid=46ca9b42-2f56-4558-a015-ffbdf9e06d57&algo_exp_id=46ca9b42-2f56-4558-a015-ffbdf9e06d57-1&pdp_npi=3%40dis%21GBP%211.75%211.23%21%21%212.19%21%21%402145265416902044100774347d0796%2112000016885769278%21sea%21UK%21765854333&curPageLogUid=e8u72WJJj6cZ) 	| 1.93 	| 1 	| 1.93 	|
| M4 Nut 	| pk of 50 	| [Aliexpress](https://www.aliexpress.com/item/4000226223259.html?spm=a2g0o.productlist.main.3.4ef41f71tQH8Dq&algo_pvid=8d08ec09-7557-4111-8432-2265dbe792c5&algo_exp_id=8d08ec09-7557-4111-8432-2265dbe792c5-1&pdp_npi=3%40dis%21GBP%210.76%210.67%21%21%210.95%21%21%402100bc5c16902047887628927d077f%2110000000892445870%21sea%21UK%21765854333&curPageLogUid=5DRXGHB10o6n) 	| 0.56 	| 1 	| 0.58 	|
| 33mm lens 	|  	| [Aliexpress](https://www.aliexpress.com/item/1713363004.html?spm=a2g0o.order_list.order_list_main.15.2e831802mm0sqm) 	| 0.45 	| 1 	| 0.45 	|
| **Electronics** 	|  	|  	|  	|  	|  	|
| Custom PCB 	| pk of 5 	| https://jlcpcb.com/ 	| 1.55 	| 1 	| 1.55 	|
| Wemos D1 Mini ESP32 	|  	| [Aliexpress](https://www.aliexpress.com/item/32858054775.html?spm=a2g0o.productlist.main.1.4b2522efJ57Pf9&algo_pvid=1bc38ca4-b8f4-4f8b-bda6-3c5912eb0c55&algo_exp_id=1bc38ca4-b8f4-4f8b-bda6-3c5912eb0c55-0&pdp_npi=3%40dis%21GBP%212.41%212.41%21%21%213.01%21%21%402145274c16902049008496968d0745%2112000021656141438%21sea%21UK%21765854333&curPageLogUid=UwZXKfyz6wW7) 	| 2.41 	| 1 	| 2.41 	|
| Resistor 220R 	|  	|  	|  	|  	|  	|
| Transistor 	|  	|  	|  	|  	|  	|
| Transistor 	|  	|  	|  	|  	|  	|
| Resistor 	|  	|  	|  	|  	|  	|
| IR LED 	|  	|  	|  	|  	|  	|
| Button 	|  	|  	|  	|  	|  	|
| Toggle Switch 	|  	|  	|  	|  	|  	|
| 2.2inch TFT 	|  	|  	|  	|  	|  	|
