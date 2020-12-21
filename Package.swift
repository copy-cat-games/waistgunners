// swift-tools-version:5.3
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "swallegro",
    products: [
        .library(name: "swallegro", targets: ["swallegro"]),
    ],
    targets: [
        // Targets are the basic building blocks of a package. A target can define a module or a test suite.
        // Targets can depend on other targets in this package, and on products in packages this package depends on.
        .target(
            name: "swallegro",
            path: "./Sources/swallegro"
        ),
        
        .target(
            name: "waistgunners",
            dependencies: [.target(name: "swallegro")]),
    ]
)
