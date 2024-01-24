import SwiftUI
import shared
import ui


// struct ContentView: View {
// 	let greet = Greeting().greet()
//
// 	var body: some View {
// 		Text(greet)
// 	}
// }

struct ComposeView: UIViewControllerRepresentable {
    func makeUIViewController(context: Context) -> UIViewController {
        MainViewControllerKt.MainViewController()
    }

    func updateUIViewController(_ uiViewController: UIViewController, context: Context) {}
}

struct ContentView: PreviewProvider {
	static var previews: some View {
		ComposeView()
	}

}




