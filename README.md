# mr_shodou
インタラクティブMR書道

## 環境構築
バージョンごとに細かな依存があるらしく?ややこしかった。
利用するバージョンを固定する。
下記が現在動作確認が行えた環境

* [Visual Studio 2017](https://www.visualstudio.com/thank-you-downloading-visual-studio/?sku=Community&rel=15)
* [UNITY 2017.4.32f1](https://unity3d.com/jp/unity/qa/lts-releases?version=2017.4)
* [MRTK 2017.4.3.0-Refresh](https://github.com/microsoft/MixedRealityToolkit-Unity/releases/tag/2017.4.3.0-Refresh)
* [Hololens Emulator 10.0.17763.134](https://go.microsoft.com/fwlink/?linkid=2065980)

* [Visual Studio 2019](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=Community&rel=16)(一応入れた。効果あるか不明)

参考リンク: https://www.slideshare.net/TakahiroMiyaura/hololenswindows-mixed-reality-128235273


## ビルドの作法
1. Unityでビルド
* `App`ディレクトリを出力先として、ビルドする
* `App`内に、Visual Studio用のソリューションが生成される

2. Visual Studioでビルド
* "Release", "x86", "リモートコンピュータ,またはエミュレータ" を選択してビルドする

## 参考プロジェクト
### for_practice/unity_cha2
#### 動作
右クリックすると四角いキューブが落下して床に乗る。同時に、カプセル錠剤みたいな形のものも起動直後に落下する。
unityチャンはデータが重すぎたので消えてもらった・・・。

#### 実行方法
for_practice/unity_cha2/App/***.slnを開いてビルド
