Pod::Spec.new do |s|
      s.name         = "fireflylib"

      s.version      = "0.0.5"

      s.summary      = "C++ base Framework"

      s.ios.deployment_target = '14.0'

      s.description  = <<-DESC
        C++ base Framework, include Thread, Time, Ringbuffer
      DESC
      s.homepage     = "https://github.com/Nevermore1994/firefly.git"
     
      s.license      = { :type => "MIT", :file => "LICENSE" }

      s.author             = { "Nevermore-" => "875229565@qq.com" }

      s.source       = { :git => "https://github.com/Nevermore1994/firefly.git", :tag => "#{s.version}" }

      s.source_files  = "framework/iOS/firefly.xcframework/**/*.{h, hpp}"

      s.public_header_files = "framework/iOS/firefly.xcframework/**/*.{h, hpp}"

      s.requires_arc = false

      s.ios.vendored_frameworks = 'framework/iOS/firefly.xcframework'
      
      s.pod_target_xcconfig = { 'ONLY_ACTIVE_ARCH' => 'YES' }
      
      s.pod_target_xcconfig = { 'EXCLUDED_ARCHS[sdk=iphonesimulator*]' => 'arm64' }

      s.user_target_xcconfig = { 'EXCLUDED_ARCHS[sdk=iphonesimulator*]' => 'arm64' }
    end