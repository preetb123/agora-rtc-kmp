import SwiftUI
import shared
// import SharedUi

struct ContentView: View {
	let greet = Greeting().greet()

	var body: some View {
		Text(greet)
	}
}

// struct ComposeView: UIViewControllerRepresentable {
//     func makeUIViewController(context: Context) -> UIViewController {
//         MainViewControllerKt.MainViewController()
//     }
//
//     func updateUIViewController(_ uiViewController: UIViewController, context: Context) {}
// }

struct ContentView_Previews: PreviewProvider {
	static var previews: some View {
		ContentView()
	}
}