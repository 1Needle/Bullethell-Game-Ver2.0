# Bullethell-Game-Ver2.0
## 作者 : 新竹高中 洪森凱
## 此為高三自主學習成果
### 檔案介紹
⚠本專案相當龐大，且分成多份檔案，建議大致瀏覽即可⚠

🔳[`main.cpp`](https://github.com/1Needle/Bullethell-Game-Ver2.0/blob/main/source%20codes/main.cpp)\
◇此檔案主要處理流程，使流程清楚易懂，各函式詳細定義都在Function.cpp中。

🔳[`Function.cpp`](https://github.com/1Needle/Bullethell-Game-Ver2.0/blob/main/source%20codes/Function.cpp)\
◇本專案的核心，包含update, processInput, draw等主要函式，並含有部分常用函式。\
◇由於SFML讀取、修改資料的需求，會有大量類似的程式碼，不須閱讀，直接跳過即可。\
◇以函式進行清楚的分工、分類，讓編寫、尋找更加容易。\
◇超過2300行程式碼，請自行斟酌閱讀。\
✳建議下載後以程式碼編輯器開啟，善用摺疊程式碼的功能，以獲得最佳的閱讀體驗。

🔳[`header files`](https://github.com/1Needle/Bullethell-Game-Ver2.0/tree/main/header%20files)\
◇本資料夾存有各類別的標頭檔，負責定義類別。

🔳[`source codes`](https://github.com/1Needle/Bullethell-Game-Ver2.0/tree/main/source%20codes)\
◇本資料夾存有各類別的原始碼，負責定義各類別之成員函式。\
◇為了方便使用，將類別設計成與SFML語法一致，，所有變數都設定為private，並透過成員函式存取，因此會有許多重複的程式碼，也建議跳過。\
◇[`main.cpp`](https://github.com/1Needle/Bullethell-Game-Ver2.0/blob/main/source%20codes/main.cpp)和[`Function.cpp`](https://github.com/1Needle/Bullethell-Game-Ver2.0/blob/main/source%20codes/Function.cpp)也存放於此。

### 遊戲介紹
◇本遊戲並未參考任何遊戲，僅以手邊擁有的素材決定製作方向。\
◇玩家角色有三個階段，可透過撿取能量來升級。\
◇共有8種敵人，每種敵人擁有不同的數值、攻擊模式、移動模式。\
◇共有10關，擊敗所有敵人即可進入下一關。\
◇有重新裝填、炸彈、追蹤彈、小地圖、HUD等特殊功能。\
◇內建**簡易地圖編輯器**(僅輸出，無讀取)、**debug模式**，可用於幫助檢查bug，加快製作遊戲。

⬇**遊戲下載**⬇\
◇[`game files`](https://github.com/1Needle/Bullethell-Game-Ver2.0/blob/main/game%20files.zip)\
◇下載後解壓縮到同一個資料夾內，開啟New bullethell.exe即可開始遊玩。

⬇**遊玩方式**⬇\
🔳**選單操作**
|**按鍵**|**功能**|
|:------:|:------:|
|WASD|選擇|
|Space|確認|
|Esc|返回/取消|

🔳**遊戲內操作**
|**按鍵**|**功能**|
|:------:|:------:|
|滑鼠|瞄準|
|滑鼠左鍵|射擊|
|滑鼠右鍵|發射炸彈|
|WASD|移動角色|
|Shift|降低移動速度|
|Esc|暫停選單|

🔳**道具介紹**
|**圖示**|**名字**|**效果**|
|:------:|:------:|:------:|
|![image](https://user-images.githubusercontent.com/102662597/162237913-4ca38d1d-01d3-4c0a-9434-ba00aeca9c00.png)|繃帶|回復10點生命值|
|![image](https://user-images.githubusercontent.com/102662597/162237958-37bb0da2-8036-43c1-a5aa-e439d7f607c2.png)|能量罐|增加能量|
|![image](https://user-images.githubusercontent.com/102662597/162237996-a927552e-ef1d-431c-aae2-bc1c39d39b72.png)|護盾|獲得可抵擋一次攻擊的護盾，持續10秒|
|![image](https://user-images.githubusercontent.com/102662597/162238034-9162b570-28e0-40ad-98f0-e30892e997a0.png)|炸彈|獲得一枚炸彈|

✳繃帶、炸彈在全滿時不會被撿起，能量罐改為獲得500分，護盾撿起會刷新持續時間。

🔳**敵人介紹**
|**圖示**|**名字**|**攻擊方式**|**移動方式**|
|:------:|:------:|:------:|:------:|
|![image](https://user-images.githubusercontent.com/102662597/162240832-ef45aa59-993c-4988-b9e8-57d3556ac50f.png)|Shotgun|同時發射四發子彈|速度中等，持續向玩家靠近|
|![image](https://user-images.githubusercontent.com/102662597/162241332-9722a1d2-8379-4de9-930d-1035230090db.png)|Beam|延遲後發射一道光束|速度緩慢，與玩家保持距離|
|![image](https://user-images.githubusercontent.com/102662597/162241682-fa5e46a4-b575-4e0b-ab8e-194c0e56ea9a.png)|Guerilla|向玩家方向快速發射子彈，準度低|速度快，繞著玩家移動|
|![image](https://user-images.githubusercontent.com/102662597/162242587-b6ea4c70-02f8-45d6-aba3-d21a9c34524a.png)|Tank|以身體碰撞|速度極慢，持續向玩家靠近|
|![image](https://user-images.githubusercontent.com/102662597/162242904-90160295-5ecf-45a0-9491-510408b4057b.png)|Madman|向四周隨機快速發射子彈|速度緩慢，持續向玩家靠近|
|![image](https://user-images.githubusercontent.com/102662597/162243519-f51630e8-cb31-45bc-ad8b-27df30680a7f.png)|Charger|向玩家快速衝撞，有冷卻|以緩慢速度靠近玩家，攻擊範圍內會衝撞|
|![image](https://user-images.githubusercontent.com/102662597/162244095-7f2cf0dd-6e26-462f-8c0b-41e0cb0039c2.png)|Battery|向四周圓形範圍連續發射兩波子彈|原地不動|
|![image](https://user-images.githubusercontent.com/102662597/162244741-2ec24e5a-54c0-4ba5-99d6-60cae2682890.png)|Mage|發射子彈環，延遲後飛向玩家|速度極快，與玩家保持距離|

‼**已知問題**‼
◇電腦配備不足可能會在最後幾關卡頓，因為子彈數量過多。
