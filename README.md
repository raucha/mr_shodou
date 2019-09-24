# mr_shodou
インタラクティブMR書道

## 環境構築
バージョンごとに細かな依存があるらしく?ややこしかった。
利用するバージョンを固定する。
下記が現在動作確認が行えた環境

* [Visual Studio 2017](https://www.visualstudio.com/thank-you-downloading-visual-studio/?sku=Community&rel=15)
* [UNITY 2017.4.32f1](https://unity3d.com/jp/unity/qa/lts-releases?version=2017.4)
* [MRTK 2017.4.3.0-Refresh](https://github.com/microsoft/MixedRealityToolkit-Unity/releases/tag/2017.4.3.0-Refresh)

* [Visual Studio 2019](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=Community&rel=16)(一応入れた。効果あるか不明)

参考リンク: https://www.slideshare.net/TakahiroMiyaura/hololenswindows-mixed-reality-128235273


## ビルドの作法
1. Unityでビルド
* `App`ディレクトリを出力先として、ビルドする
* `App`内に、Visual Studio用のソリューションが生成される

2. Visual Studioでビルド
* "Release", "x86", "リモートコンピュータ,またはエミュレータ" を選択してビルドする
