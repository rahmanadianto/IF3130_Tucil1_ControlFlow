# IF3130_Tucil1_ControlFlow
Tugas kecil mata kuliah Jaringan Komputer

OS : Linux

Usage :
- transmitter
	transmitter [receiver IP] [receiver port] [message.txt]
- receiver
	receiver [port]

1. receiver IP selalu localhost (127.0.0.1)
2. receiver harus dijalankan dahulu sebelum transmitter
3. masuk folder bin
4. run receiver dengan sebuah terminal
5. run transmitter dengan terminal yang lain
6. jika kedua program tidak dapat dirun, lakukan langkah compile
7. message.txt berisi pesan yang akan dikirim

Compile :
- transmitter :
	cd [to the source code directory]
	make transmitter
- receiver :
	cd [to the source code directory]
	make receiver
