# Flappy Bird Clone - Báo Cáo Dự Án

## 1. Gameplay - Lối chơi, Logic của game
Game **Flappy Bird Clone** là một trò chơi arcade đơn giản, nơi người chơi điều khiển một chú chim bay qua các chướng ngại vật. Cơ chế chính của trò chơi là nhấn **Space** để giúp chim bay lên; nếu không nhấn, chim sẽ rơi xuống do trọng lực.

### **Cơ chế hoạt động**
- Người chơi nhấn phím **Space** để làm cho nhân vật chim bay lên. Nếu không nhấn, chim sẽ rơi tự do.
- Chim phải vượt qua các ống cản trở. Nếu va chạm vào ống hoặc rơi xuống đất, trò chơi kết thúc.
- Người chơi nhận điểm khi vượt qua mỗi cặp ống thành công.
- Các vật phẩm đặc biệt có thể xuất hiện, giúp tăng điểm hoặc tạo hiệu ứng đặc biệt.
- Tốc độ của game tăng dần theo thời gian, làm tăng độ khó.

## 2. Đồ họa, Âm thanh

### **Đồ họa**
- Đồ họa **2D** đơn giản, phong cách pixel art.
- **Background** có thể cuộn ngang để tạo cảm giác chuyển động.
- Hiệu ứng hạt khi chim va chạm hoặc thu thập vật phẩm.
- Hình ảnh của chim, ống và vật phẩm được thiết kế đơn giản nhưng rõ ràng.

### **Âm thanh**
- Hiệu ứng âm thanh khi nhấn phím để chim bay.
- Âm thanh khi va chạm với ống hoặc mặt đất.
- Nhạc nền nhẹ nhàng, lặp lại.
- Hiệu ứng âm thanh khi nhận vật phẩm hoặc đạt điểm mới.

## 3. Cấu trúc của project game

### **3.1. File chính**
- **main.cpp:**
  - Khởi chạy chương trình.
  - Khởi tạo SDL và các hệ thống con.
  - Chứa vòng lặp game chính.
  - Xử lý các sự kiện cơ bản.

### **3.2. Modules game**
- **Game.h/Game.cpp:**
  - Quản lý logic game tổng thể.
  - Điều phối các thành phần khác.
  - Quản lý trạng thái game (**menu, gameplay, game over**).
  - Xử lý tính điểm và độ khó.

- **Bird.h/Bird.cpp:**
  - Định nghĩa class **Bird**.
  - Xử lý logic bay, trọng lực.
  - Animation và hiệu ứng đặc biệt.

- **Pipe.h/Pipe.cpp:**
  - Định nghĩa class **Pipe**.
  - Quản lý vị trí và chuyển động ống.
  - Xử lý va chạm với chim.

- **Item.h/Item.cpp:**
  - Quản lý các **power-up** (tăng điểm, thu nhỏ).
  - Xử lý hiệu ứng khi thu thập item.

- **Render.h/Render.cpp:**
  - Quản lý hệ thống **render**.
  - Xử lý vẽ background, đối tượng, điểm số.

- **Screen.h/Screen.cpp:**
  - Quản lý các màn hình (**start, game over, pause**).
  - Xử lý input trên các màn hình.

## 4. Các chức năng đã cài đặt
- ✅ Chim có thể bay bằng cách nhấn phím.
- ✅ Ống di chuyển, tính điểm.
- ✅ Vật phẩm đặc biệt (**+ điểm, thu nhỏ**).
- ✅ Nhạc nền, hiệu ứng âm thanh.
- ✅ Cơ chế tăng độ khó theo thời gian.
- ✅ có lưu lại điểm sau mỗi lần chơi.
- 

---
💡 *README này sử dụng Markdown để hiển thị đẹp trên GitHub.*
