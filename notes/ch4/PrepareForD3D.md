# 準備工作


* [Direct3D](#Driect3D)
* [COM組件](#COM)
* [紋理與數據資源格式](#Texture-data-format)
* [交換鏈與頁面翻轉](#swap-chain-and-page-flipping)
* [深度緩衝區](#depth-buffer)

<h2 id="Driect3D">Direct3D</h2>

位於繪圖硬體以及軟體之間的API

<h2 id="COM">COM組件</h2>

微軟提出的開發框架，以純虛函式類別實現物件導向

每個類別都繼承IUnknow介面

    // 只能有虛函式成員
    #define struct interface
    interface IUnknow{...}

1. 透過使用特定的函數而非C++的new關鍵字來建立物件

2. 當不使用時使用Release()釋放(組件所在模組自行管理物件的生命週期)，而不使用delte關鍵字

<h2 id="Texture-data-format">紋理與數據資源格式</h2>

1. 2DTexture是一種數據元素矩陣，用途之一是儲存2D圖像數據

2. 在紋理中的每個元素中儲存一個像素顏色。但不是Texture唯一用途，例如在法線貼圖(normal texture)，儲存的是3D向量。

3. 1DTexture是一個一維陣列，3DTexture是三維陣列。

4. GPU可以在Texture上做特殊運算，例如fliter(過濾器)、multisampling(多重採樣，鋸齒用)

5. 常用格式為，RGBA代表Red、Green、Blue、Alpha(透明度):

    * Texture只支持特定格式的資料儲存，這些格式由DXGI_FORMAT列舉值決定

    * D XGI_FORMAT_R32G32B32_FLOAT：每個元素包含3個32位浮點分量。

    * DXGI_FORMAT_R16G16B16A16_UNORM：每個元素包含4個16位分量，分量的取值範圍在[0,1]區間內。

    * DXGI_FORMAT_R32G32_UINT：每個元素包含兩個32位無符號整數分量。
    * DXGI_FORMAT_R8G8B8A8_UNORM：每個元素包含4個8位無符號分量，分量的取值範圍在[0,1]區間內。

    * DXGI_FORMAT_R8G8B8A8_SNORM：每個元素包含4個8位有符號分量，分量的取值範圍在[−1,1] 區間內。

    * DXGI_FORMAT_R8G8B8A8_SINT：每個元素包含4個8位有符號整數分量，分量的取值範圍在[−128, 127] 區間內。

    * DXGI_FORMAT_R8G8B8A8_UINT：每個元素包含4個8位無符號整數分量，分量的取值範圍在[0, 255]區間內。

6. 如上述，Texture不一定是顏色訊息，格式DXGI_FORMAT_R32G32B32_FLOAT可以用來表示3d float向量

7. 有一種弱類型格式，可以預先分配記憶體空間，然後在Texture綁定時在決定如何解釋數據內容:例如
    * DXGI_FORMAT_R8G8B8A8_TYPELESS
預先預留4個八位芬輛，不指定為整數、浮點數等

<h2 id="swap-chain-and-page-flipping">交換鏈與頁面翻轉</h2>

電腦渲染畫面有時在顯示器更新前就能完成，這樣的話會導致螢幕顯示未渲染完成的畫面

解決方案便是使用兩個Texture做為顏色緩存，分為**前台緩衝(front buffer)**與**後台緩衝(back buffer)**。

前台緩衝區紀錄目前這一幀顏色訊息，下一幀則在後台緩稱區執行渲染，當選渲染完成後，兩個緩衝區翻轉，前台變後台，後台變前台...

翻轉僅是將兩邊的指標做交換，是很快的動作稱為**呈現(presenting)**。

![image](../pic/ch4/4.1.jpg)

* 這樣前後台緩衝區便形成的**交換鏈(swap chain)**

* 使用兩個緩衝稱為**雙緩衝(double buffering)**

<h2 id="depth-buffer">深度緩衝區</h2>

