# CandyLand
Đây là dự án game cá nhân của mình được tạo bới ngôn ngữ C++ và SDL2. Dự án này là một bài tập lớn của môn học Lập Trình Nâng Cao tại trường Đại học Công Nghệ, VNU Hà Nội.
## 🕵️ Mục lục
1. [Thông tin](#introduction)
2. [Ý tưởng](#paragraph1)
3. [Cách chơi](#paragraph2)
4. [Cách cài đặt](#paragraph3)
5. [Nguồn](#paragraph4)

## ℹ Thông tin <a name="introduction"></a>
- Họ và tên: Vương Phương Thảo
- Mã sinh viên: 22026519
- Lớp: K67J
- Email: vuongphuongthao6a6yh@gmail.com

## 💡 Ý tưởng  <a name="paragraph1"></a>
Trò chơi được lấy ý tưởng từ tưởng từ trò chơi nổi tiếng Candy Crush Saga 🍬🍭🍫

## 🎮 Cách chơi  <a name="paragraph2"></a>
- Sử dụng chuột 🖱 ️để hoán đổi 2 vật phẩm liền kề trong bàn cờ
- Nếu bạn tạo được chuỗi kẹo liên tiếp >= 3 theo chiều ngang hoặc theo chiều dọc, các viên kẹo sẽ biến mất.

- Game có 6 viên kẹo thường và 3 viên đặc biệt 
    - Chuỗi 5 sẽ tạo ra 1 viên ![image](https://user-images.githubusercontent.com/125387948/236450472-7be5a03f-2350-4622-a80e-be03b825aa9d.png) `Star`
    - Tại 1 vị trí tạo thánh cả chuỗi 3 dọc và ngang thì ta sẽ có được 1 viên ![image](https://user-images.githubusercontent.com/125387948/236450734-9af55913-12ac-4e90-a0fd-7af5effaf242.png) `Bomb`
    - Chuỗi 4 sẽ tạo ra 1 viên ![image](https://user-images.githubusercontent.com/125387948/236450944-0fa1c885-084f-4168-ada5-bccceec32fca.png) `Striped`

- Khi bạn đổi chỗ viên `Star` với viên kẹo thường, tất cả viên kẹo cùng loại với viên kẹo đó sẽ biến mất. Còn khi kết hợp 2 viên `Star`, tất cả viên kẹo sẽ biến mất
- Để ăn được viên `Bomb` bạn chỉ cần click vào và 8 viên kẹo xung quanh đó sẽ biến mất.
- Cuối cùng là viên `Striped`, cả 1 hàng ngang hoặc dọc sẽ biến mất theo thao tác của bạn. Nếu bạn di chuyển viên Striped theo hàng dọc thì cả 1 hàng dọc sẽ biến mất và điều đó tương tự với hàng ngang.
- Trong phạm vi viên `Bomb` và `Striped` đi qua nếu có viên đặc biệt bất kỳ thì những viên đặc biệt này cũng sẽ bị ăn và có hiệu ứng tương tự.
- 1 ⭐ ~ 150 điểm 

## 📩 Cách cài đặt  <a name="paragraph3"></a>
Tải về `link` và giải nén nó
Chạy file CandLand.exe.

## Nguồn  <a name="paragraph4"></a>
- [start](https://www.freepik.com/free-vector/colorful-candy-land-background_2306400.htm)
- [background](https://www.freepik.com/premium-vector/sweet-candy-land-with-cloud-background_32610429.htm)
- [candy 1](https://www.freepik.com/free-vector/candy-game-icons-confectionery-pastry-set_24315603.htm#query=candy%20game%20icon&position=0&from_view=search&track=ais)
- [candy 2](https://www.freepik.com/free-vector/candy-game-icons-cartoon-vector-ui-sweets-set_24131294.htm?fbclid=IwAR3lyADetSug4BB96OJaybbnpEc_Mn-WrKl5NxJgbPc7Mn0EO3QQmDJqqe4)
- [button](https://www.freepik.com/premium-vector/set-buttons-custom-cartoonstyle-game-interface_30670670.htm)
- [animation](https://www.freepik.com/free-vector/cartoon-element-animation-frames_13818851.htm?query=bomb%20animation)
- [Music](https://www.youtube.com/watch?v=5NvZ7xuzLI4)
