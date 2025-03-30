1. Gameplay - Lối chơi, Logic của game
Game "Flappy Bird Clone" là một trò chơi arcade đơn giản, nơi người chơi điều khiển một chú chim bay qua các chướng ngại vật. Cơ chế chính của trò chơi là nhấn Space để giúp chim bay lên, nếu không chim sẽ rơi xuống do trọng lực.
Cơ chế hoạt động:
Người chơi nhấn phím để làm cho nhân vật chim bay lên, nếu không nhấn, chim sẽ rơi tự do.
Chim phải vượt qua các ống cản trở, nếu va chạm vào ống hoặc rơi xuống đất, trò chơi kết thúc.
Người chơi nhận điểm khi vượt qua mỗi cặp ống thành công.
Các vật phẩm có thể xuất hiện, giúp tăng điểm hoặc tạo hiệu ứng đặc biệt.
Tốc độ của game có thể tăng dần theo thời gian, làm tăng độ khó.
2. Đồ họa, Âm thanh
Đồ họa:
Đồ họa 2D đơn giản, phong cách pixel art.
Background có thể cuộn ngang để tạo cảm giác chuyển động.
Hiệu ứng hạt khi chim va chạm hoặc thu thập vật phẩm.
Hình ảnh của chim, ống và vật phẩm đều được thiết kế đơn giản nhưng rõ ràng.
Âm thanh:
Hiệu ứng âm thanh khi nhấn phím để chim bay.
Âm thanh khi va chạm với ống hoặc mặt đất.
Nhạc nền nhẹ nhàng, lặp lại.
Hiệu ứng âm thanh khi nhận vật phẩm hoặc đạt điểm mới.
3. Cấu trúc của project game
main.cpp:
File chính khởi chạy chương trình
Khởi tạo SDL và các hệ thống con
Chứa vòng lặp game chính
Xử lý các sự kiện cơ bản
Game.h/Game.cpp:
Quản lý logic game tổng thể
Điều phối các thành phần khác
Quản lý trạng thái game (menu, gameplay, game over)
Xử lý tính điểm và độ khó
2.2. Object Modules
Bird.h/Bird.cpp:
Định nghĩa class Bird
Xử lý logic bay, trọng lực
Animation và hiệu ứng đặc biệt
Quản lý trạng thái (shrunk, falling)
Pipe.h/Pipe.cpp:
Định nghĩa class Pipe
Quản lý vị trí và chuyển động ống
Xử lý va chạm với chim
Logic ống di chuyển lên xuống
Item.h/Item.cpp:
Định nghĩa class Item
Quản lý các power-up (điểm cộng, thu nhỏ)
Xử lý hiệu ứng khi thu thập item
2.3. Rendering Modules
Render.h/Render.cpp:
Quản lý toàn bộ hệ thống render
Xử lý vẽ background, các đối tượng
Hiển thị điểm số, thông báo
Hiệu ứng đặc biệt (screen shake)
Screen.h/Screen.cpp:
Quản lý các màn hình (start, game over, pause)
Xử lý input trên các màn hình
Hiển thị hướng dẫn và nút bấm
2.4. Effect Modules
SmokePartici.h/SmokePartici.cpp:
Hệ thống particle effect khói
Xử lý khi chim rơi
Quản lý vòng đời particle
SparklePartici.h/SparklePartici.cpp:
Hệ thống particle effect lấp lánh
Hiệu ứng khi ghi điểm
Animation và fade out
4. Các chức năng đã cài được cho game
Chim có thể bay bằng cách nhấn phím.
Ống xuất hiện và di chuyển từ phải sang trái.
Khi chim vượt qua ống, điểm số tăng lên.
Vật phẩm đặc biệt xuất hiện ngẫu nhiên, cung cấp hiệu ứng đặc biệt.(+ Điểm,thu nhỏ)
Va chạm với ống hoặc rơi xuống đất sẽ kết thúc trò chơi.
Hiệu ứng hạt khi va chạm hoặc nhận vật phẩm.
Nhạc nền và hiệu ứng âm thanh.
Cơ chế tăng độ khó theo thời gian (tăng tốc độ game).

