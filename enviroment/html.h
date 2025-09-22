#ifndef HTML_H
#define HTML_H

const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 Air Quality Monitor</title>
    <script src="https://cdn.jsdelivr.net/npm/chart.js@4.4.0/dist/chart.min.js"></script>
    <style>
        :root {
            --bg-primary: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            --bg-secondary: rgba(255, 255, 255, 0.95);
            --bg-card: rgba(255, 255, 255, 0.9);
            --text-primary: #1a202c;
            --text-secondary: #4a5568;
            --border: rgba(255, 255, 255, 0.2);
            --shadow: 0 8px 32px rgba(0, 0, 0, 0.1);
            --shadow-lg: 0 20px 40px rgba(0, 0, 0, 0.15);
            --accent-primary: #667eea;
            --accent-secondary: #764ba2;
            --glass-bg: rgba(255, 255, 255, 0.25);
            --glass-border: rgba(255, 255, 255, 0.18);
        }

        [data-theme="dark"] {
            --bg-primary: linear-gradient(135deg, #0f0f23 0%, #1a1a2e 50%, #16213e 100%);
            --bg-secondary: rgba(16, 16, 35, 0.95);
            --bg-card: rgba(26, 26, 46, 0.9);
            --text-primary: #e2e8f0;
            --text-secondary: #a0aec0;
            --border: rgba(255, 255, 255, 0.1);
            --shadow: 0 8px 32px rgba(0, 0, 0, 0.5);
            --shadow-lg: 0 20px 40px rgba(0, 0, 0, 0.6);
            --accent-primary: #bb86fc;
            --accent-secondary: #03dac6;
            --glass-bg: rgba(255, 255, 255, 0.05);
            --glass-border: rgba(255, 255, 255, 0.08);
        }

        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
            transition: background-color 0.4s ease, color 0.4s ease, border-color 0.4s ease;
        }

        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
            background: var(--bg-primary);
            color: var(--text-primary);
            line-height: 1.6;
            min-height: 100vh;
            position: relative;
        }

        .floating-particles {
            position: fixed;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
            pointer-events: none;
            z-index: -1;
        }

        .particle {
            position: absolute;
            width: 4px;
            height: 4px;
            background: var(--accent-primary);
            border-radius: 50%;
            animation: float-particle 15s linear infinite;
            opacity: 0.6;
        }

        @keyframes float-particle {
            0% {
                transform: translateY(100vh) rotate(0deg);
                opacity: 0;
            }
            10% {
                opacity: 0.6;
            }
            90% {
                opacity: 0.6;
            }
            100% {
                transform: translateY(-100vh) rotate(360deg);
                opacity: 0;
            }
        }

        .container {
            max-width: 1200px;
            margin: 0 auto;
            padding: 20px;
        }

        .header {
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin-bottom: 30px;
            padding: 24px;
            background: var(--glass-bg);
            backdrop-filter: blur(20px);
            -webkit-backdrop-filter: blur(20px);
            border-radius: 24px;
            box-shadow: var(--shadow-lg);
            border: 1px solid var(--glass-border);
        }

        .logo {
            font-size: 24px;
            font-weight: bold;
            background: linear-gradient(135deg, var(--accent-primary), var(--accent-secondary));
            background-clip: text;
            -webkit-background-clip: text;
            -webkit-text-fill-color: transparent;
            cursor: pointer;
            animation: float 3s ease-in-out infinite;
        }

        @keyframes float {
            0%, 100% {
                transform: translateY(0px);
            }
            50% {
                transform: translateY(-10px);
            }
        }

        .connection-status {
            background: var(--glass-bg);
            backdrop-filter: blur(16px);
            -webkit-backdrop-filter: blur(16px);
            border: 2px solid var(--glass-border);
            color: var(--text-primary);
            padding: 12px 20px;
            border-radius: 50px;
            font-weight: 600;
            font-size: 14px;
            display: flex;
            align-items: center;
            gap: 8px;
        }

        .connection-status.connected {
            border-color: #22c55e;
            color: #22c55e;
        }

        .connection-status.disconnected {
            border-color: #ef4444;
            color: #ef4444;
        }

        .theme-toggle {
            background: var(--glass-bg);
            backdrop-filter: blur(16px);
            -webkit-backdrop-filter: blur(16px);
            border: 2px solid var(--glass-border);
            color: var(--text-primary);
            padding: 12px 20px;
            border-radius: 50px;
            cursor: pointer;
            font-weight: 600;
            font-size: 14px;
            display: flex;
            align-items: center;
            gap: 8px;
            transition: all 0.3s ease;
        }

        .theme-toggle:hover {
            background: var(--accent-primary);
            color: white;
            transform: translateY(-2px);
        }

        .alert-card {
            background: linear-gradient(135deg, #ef4444, #dc2626);
            color: white;
            padding: 20px;
            border-radius: 20px;
            text-align: center;
            margin-bottom: 20px;
            animation: pulse 2s infinite;
            display: none;
        }

        @keyframes pulse {
            0%, 100% { transform: scale(1); }
            50% { transform: scale(1.02); }
        }

        .top-section {
            display: grid;
            grid-template-columns: 1fr 2fr;
            gap: 20px;
            margin-bottom: 30px;
        }

        .aqi-card {
            background: var(--glass-bg);
            backdrop-filter: blur(20px);
            -webkit-backdrop-filter: blur(20px);
            padding: 30px;
            border-radius: 24px;
            box-shadow: var(--shadow-lg);
            text-align: center;
            border: 1px solid var(--glass-border);
            transition: all 0.3s ease;
        }

        .aqi-card:hover {
            transform: translateY(-8px) scale(1.02);
            box-shadow: 0 30px 60px rgba(0, 0, 0, 0.2);
        }

        .aqi-number {
            font-size: 48px;
            font-weight: bold;
            margin-bottom: 10px;
            background: linear-gradient(135deg, var(--accent-primary), var(--accent-secondary));
            background-clip: text;
            -webkit-background-clip: text;
            -webkit-text-fill-color: transparent;
        }

        .aqi-level {
            font-size: 18px;
            font-weight: 600;
            margin-bottom: 15px;
            text-transform: uppercase;
            letter-spacing: 1px;
        }

        .aqi-indicator {
            width: 20px;
            height: 20px;
            border-radius: 50%;
            margin: 0 auto 15px;
            animation: breathe 2s ease-in-out infinite;
        }

        @keyframes breathe {
            0%, 100% {
                transform: scale(1);
            }
            50% {
                transform: scale(1.1);
            }
        }

        .updated-time {
            color: var(--text-secondary);
            font-size: 14px;
        }

        .pm-section {
            background: var(--glass-bg);
            backdrop-filter: blur(20px);
            -webkit-backdrop-filter: blur(20px);
            padding: 24px;
            border-radius: 24px;
            box-shadow: var(--shadow-lg);
            border: 1px solid var(--glass-border);
            transition: all 0.3s ease;
        }

        .pm-section:hover {
            transform: translateY(-6px) scale(1.01);
        }

        .section-title {
            font-size: 18px;
            font-weight: 600;
            margin-bottom: 15px;
            background: linear-gradient(135deg, var(--text-primary), var(--accent-primary));
            background-clip: text;
            -webkit-background-clip: text;
            -webkit-text-fill-color: transparent;
        }

        .pm-values {
            display: grid;
            grid-template-columns: 1fr 1fr;
            gap: 20px;
            margin-bottom: 20px;
        }

        .pm-value {
            text-align: center;
            padding: 20px;
            background: var(--glass-bg);
            backdrop-filter: blur(10px);
            -webkit-backdrop-filter: blur(10px);
            border-radius: 16px;
            border: 1px solid var(--glass-border);
            transition: all 0.3s ease;
        }

        .pm-value:hover {
            transform: translateY(-3px);
        }

        .pm-number {
            font-size: 24px;
            font-weight: bold;
            background: linear-gradient(135deg, var(--text-primary), var(--accent-primary));
            background-clip: text;
            -webkit-background-clip: text;
            -webkit-text-fill-color: transparent;
            animation: gentle-pulse 4s ease-in-out infinite;
        }

        @keyframes gentle-pulse {
            0%, 100% {
                transform: scale(1);
                opacity: 1;
            }
            50% {
                transform: scale(1.05);
                opacity: 0.9;
            }
        }

        .pm-unit {
            color: var(--text-secondary);
            font-size: 14px;
        }

        .chart-container {
            position: relative;
            height: 200px;
            width: 100%;
            margin-top: 20px;
            border-radius: 16px;
            overflow: hidden;
            background: var(--glass-bg);
            backdrop-filter: blur(10px);
            -webkit-backdrop-filter: blur(10px);
            border: 1px solid var(--glass-border);
            padding: 10px;
            display: flex;
            align-items: center;
            justify-content: center;
        }

        .gas-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(180px, 1fr));
            gap: 20px;
            margin-bottom: 30px;
        }

        .gas-card {
            background: var(--glass-bg);
            backdrop-filter: blur(16px);
            -webkit-backdrop-filter: blur(16px);
            padding: 24px;
            border-radius: 20px;
            box-shadow: var(--shadow);
            text-align: center;
            border: 1px solid var(--glass-border);
            transition: all 0.3s ease;
            cursor: pointer;
        }

        .gas-card:hover {
            transform: translateY(-6px) scale(1.03);
            box-shadow: 0 25px 50px rgba(0, 0, 0, 0.25);
        }

        .gas-name {
            font-size: 14px;
            font-weight: 600;
            color: var(--text-secondary);
            margin-bottom: 8px;
        }

        .gas-value {
            font-size: 24px;
            font-weight: bold;
            background: linear-gradient(135deg, var(--text-primary), var(--accent-primary));
            background-clip: text;
            -webkit-background-clip: text;
            -webkit-text-fill-color: transparent;
        }

        .gas-unit {
            font-size: 12px;
            color: var(--text-secondary);
            margin-left: 4px;
        }

        .gauge {
            width: 100%;
            height: 8px;
            background: var(--bg-secondary);
            border-radius: 4px;
            margin-top: 10px;
            overflow: hidden;
        }

        .gauge-fill {
            height: 100%;
            border-radius: 4px;
            background: linear-gradient(90deg, var(--accent-secondary), var(--accent-primary));
            transition: all 0.8s ease;
        }

        .environmental-section {
            background: var(--glass-bg);
            backdrop-filter: blur(20px);
            -webkit-backdrop-filter: blur(20px);
            padding: 24px;
            border-radius: 24px;
            box-shadow: var(--shadow-lg);
            margin-bottom: 30px;
            border: 1px solid var(--glass-border);
            transition: all 0.3s ease;
        }

        .environmental-section:hover {
            transform: translateY(-6px) scale(1.01);
        }

        .env-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(150px, 1fr));
            gap: 20px;
        }

        .env-item {
            text-align: center;
            padding: 20px;
            background: var(--glass-bg);
            backdrop-filter: blur(10px);
            -webkit-backdrop-filter: blur(10px);
            border-radius: 16px;
            border: 1px solid var(--glass-border);
            transition: all 0.3s ease;
        }

        .env-item:hover {
            transform: translateY(-3px);
        }

        .env-label {
            font-size: 14px;
            color: var(--text-secondary);
            margin-bottom: 5px;
            font-weight: 500;
        }

        .env-value {
            font-size: 20px;
            font-weight: bold;
            background: linear-gradient(135deg, var(--text-primary), var(--accent-primary));
            background-clip: text;
            -webkit-background-clip: text;
            -webkit-text-fill-color: transparent;
            animation: gentle-pulse 4s ease-in-out infinite;
        }

        .good {
            color: #22c55e;
        }

        .moderate {
            color: #f59e0b;
        }

        .unhealthy {
            color: #ef4444;
        }

        /* AI Chatbot Styles */
        .chatbot-container {
            position: fixed;
            bottom: 20px;
            right: 20px;
            width: 350px;
            height: 500px;
            background: var(--glass-bg);
            backdrop-filter: blur(20px);
            -webkit-backdrop-filter: blur(20px);
            border-radius: 20px;
            box-shadow: var(--shadow-lg);
            border: 1px solid var(--glass-border);
            display: none;
            flex-direction: column;
            z-index: 1000;
        }

        .chatbot-header {
            background: linear-gradient(135deg, var(--accent-primary), var(--accent-secondary));
            color: white;
            padding: 15px;
            border-radius: 20px 20px 0 0;
            display: flex;
            justify-content: space-between;
            align-items: center;
        }

        .chatbot-title {
            font-weight: 600;
            font-size: 16px;
        }

        .chatbot-close {
            background: none;
            border: none;
            color: white;
            font-size: 20px;
            cursor: pointer;
            padding: 5px;
        }

        .chatbot-messages {
            flex: 1;
            padding: 15px;
            overflow-y: auto;
            display: flex;
            flex-direction: column;
            gap: 10px;
        }

        .message {
            padding: 10px 15px;
            border-radius: 15px;
            max-width: 80%;
            word-wrap: break-word;
        }

        .message.user {
            background: var(--accent-primary);
            color: white;
            align-self: flex-end;
        }

        .message.bot {
            background: var(--glass-bg);
            color: var(--text-primary);
            align-self: flex-start;
            border: 1px solid var(--glass-border);
        }

        .chatbot-input-container {
            padding: 15px;
            border-top: 1px solid var(--glass-border);
        }

        .chatbot-input {
            width: 100%;
            padding: 10px 15px;
            border: 1px solid var(--glass-border);
            border-radius: 25px;
            background: var(--glass-bg);
            color: var(--text-primary);
            font-size: 14px;
            outline: none;
        }

        .chatbot-input:focus {
            border-color: var(--accent-primary);
        }

        .chatbot-toggle {
            position: fixed;
            bottom: 20px;
            right: 20px;
            width: 60px;
            height: 60px;
            background: linear-gradient(135deg, var(--accent-primary), var(--accent-secondary));
            border: none;
            border-radius: 50%;
            color: white;
            font-size: 24px;
            cursor: pointer;
            box-shadow: var(--shadow-lg);
            transition: all 0.3s ease;
            z-index: 1001;
        }

        .chatbot-toggle:hover {
            transform: scale(1.1);
        }

        @media (max-width: 768px) {
            .top-section {
                grid-template-columns: 1fr;
            }
            .gas-grid {
                grid-template-columns: repeat(auto-fit, minmax(150px, 1fr));
            }
            .header {
                flex-direction: column;
                gap: 16px;
            }
            .chatbot-container {
                width: calc(100vw - 40px);
                height: calc(100vh - 40px);
                bottom: 20px;
                right: 20px;
            }
        }
    </style>
</head>
<body>
    <div class="floating-particles"></div>
    
    <div class="container">
        <div class="header">
            <div class="logo" onclick="createConfetti()">ESP32 Air Quality Monitor</div>
            <div class="connection-status" id="connectionStatus">
                <span id="statusIcon">📡</span>
                <span id="statusText">Connecting...</span>
            </div>
            <button class="theme-toggle" onclick="toggleTheme()" id="themeToggle">
                <span id="themeIcon">🌙</span>
                <span id="themeText">Dark Mode</span>
            </button>
        </div>
        
        <div id="alertCard" class="alert-card">
            <h3>⚠️ AIR QUALITY ALERT</h3>
            <p id="alertMessage">Dangerous conditions detected!</p>
        </div>
        
        <div class="top-section">
            <div class="aqi-card">
                <div class="aqi-number" id="aqiNumber">--</div>
                <div class="aqi-level" id="aqiLevel">Loading...</div>
                <div class="aqi-indicator" id="aqiIndicator"></div>
                <div class="updated-time" id="updatedTime">Initializing...</div>
            </div>
            
            <div class="pm-section">
                <div class="section-title">🌫️ Air Quality Data</div>
                <div class="pm-values">
                    <div class="pm-value">
                        <div class="pm-number" id="mq135Ppm">--</div>
                        <div class="pm-unit">PPM (MQ135)</div>
                    </div>
                    <div class="pm-value">
                        <div class="pm-number" id="mq135Raw">--</div>
                        <div class="pm-unit">Raw ADC</div>
                    </div>
                </div>
                <div class="chart-container">
                    <canvas id="pmChart"></canvas>
                </div>
            </div>
        </div>
        
        <div class="section-title">🧪 Gas Measurements</div>
        <div class="gas-grid">
            <div class="gas-card" onclick="highlightCard(this)">
                <div class="gas-name">CO (Carbon Monoxide)</div>
                <div class="gas-value" id="coStatus">--</div>
                <div class="gauge">
                    <div class="gauge-fill" id="coGauge" style="width: 0%;"></div>
                </div>
            </div>
            <div class="gas-card" onclick="highlightCard(this)">
                <div class="gas-name">Air Quality Level</div>
                <div class="gas-value" id="airQualityStatus">--</div>
                <div class="gauge">
                    <div class="gauge-fill" id="airQualityGauge" style="width: 0%;"></div>
                </div>
            </div>
        </div>
        
        <div class="environmental-section">
            <div class="section-title">🌡️ Environmental Conditions</div>
            <div class="env-grid">
                <div class="env-item">
                    <div class="env-label">Temperature</div>
                    <div class="env-value" id="temperature">--°C</div>
                </div>
                <div class="env-item">
                    <div class="env-label">Pressure</div>
                    <div class="env-value" id="pressure">-- hPa</div>
                </div>
                <div class="env-item">
                    <div class="env-label">Altitude</div>
                    <div class="env-value" id="altitude">-- m</div>
                </div>
            </div>
        </div>
    </div>

    <!-- AI Chatbot -->
    <button class="chatbot-toggle" onclick="toggleChatbot()" id="chatbotToggle">🤖</button>
    
    <div class="chatbot-container" id="chatbotContainer">
        <div class="chatbot-header">
            <div class="chatbot-title">Air Quality AI Assistant</div>
            <button class="chatbot-close" onclick="toggleChatbot()">×</button>
        </div>
        <div class="chatbot-messages" id="chatbotMessages">
            <div class="message bot">
                Hello! I'm your Air Quality AI Assistant. I can help you understand:
                <br>• Carbon Monoxide (CO) safety
                <br>• Ammonia (NH₃) exposure limits
                <br>• Carbon Dioxide (CO₂) effects
                <br>• Air Quality Index (AQI) guidelines
                <br><br>Ask me anything about air quality and gas safety!
            </div>
        </div>
        <div class="chatbot-input-container">
            <input type="text" class="chatbot-input" id="chatbotInput" placeholder="Ask about air quality..." onkeypress="handleChatbotKeypress(event)">
        </div>
    </div>

    <script>
        let currentTheme = 'light';
        let pmChart = null;
        let dataHistory = [];
        let isConnected = false;
        
        // AI Chatbot Knowledge Base
        const aiKnowledge = {
            "co": {
                keywords: ["carbon monoxide", "co", "carbon monoxide safety", "co levels", "co poisoning"],
                responses: [
                    "🔴 Carbon Monoxide (CO) is a colorless, odorless, tasteless gas that's extremely dangerous.",
                    "Safe exposure limits: WHO guideline is 9 ppm (8-hour average), 25 ppm (1-hour average).",
                    "Symptoms: 10-20 ppm causes mild headache and tiredness. 35-50 ppm causes dizziness and nausea. 100 ppm can cause confusion, unconsciousness, or death.",
                    "Emergency actions: Leave the area immediately, ventilate the space, call emergency services, and use CO detectors.",
                    "Sources: Incomplete burning of fuels, vehicle exhaust, gas stoves, heaters, furnaces, and cigarette smoke."
                ]
            },
            "nh3": {
                keywords: ["ammonia", "nh3", "ammonia safety", "ammonia exposure", "fertilizer"],
                responses: [
                    "🟢 Ammonia (NH₃) is a colorless gas with a sharp, pungent odor.",
                    "Safe exposure limits: OSHA 25 ppm (8-hour limit), 35 ppm (15-minute short-term limit).",
                    "Symptoms: Low levels cause eye, nose, throat irritation. High levels can cause difficulty breathing, chest pain, and lung damage.",
                    "Emergency actions: Move to fresh air immediately, use ammonia-rated filters, rinse skin/eyes with water if direct contact occurs.",
                    "Sources: Agricultural fertilizers, livestock waste, industrial refrigeration, and cleaning products."
                ]
            },
            "co2": {
                keywords: ["carbon dioxide", "co2", "carbon dioxide safety", "co2 levels", "greenhouse gas"],
                responses: [
                    "🟠 Carbon Dioxide (CO₂) is a colorless, odorless gas that's a major greenhouse gas.",
                    "Safe exposure limits: Indoor recommended <1000 ppm, OSHA workplace limit 5000 ppm (8-hour average).",
                    "Symptoms: 1000-2000 ppm causes drowsiness and headache. 5000 ppm causes increased heart rate and dizziness. 40,000 ppm can cause unconsciousness or death.",
                    "Emergency actions: Ventilate the area, use fans or air purifiers with outdoor air intake.",
                    "Environmental impact: Major contributor to global warming, climate change, and ocean acidification."
                ]
            },
            "aqi": {
                keywords: ["aqi", "air quality index", "air quality levels", "air quality guidelines"],
                responses: [
                    "📊 Air Quality Index (AQI) Guidelines:",
                    "0-50 (Good): Air is safe for everyone.",
                    "51-100 (Moderate): Sensitive people should limit outdoor activity.",
                    "101-150 (Unhealthy for Some): People with lung/heart issues should stay indoors.",
                    "151-200 (Unhealthy): Everyone should limit outdoor activity.",
                    "201-300 (Very Unhealthy): Avoid all outdoor activities.",
                    "301+ (Hazardous): Stay indoors with air filtration."
                ]
            },
            "general": {
                keywords: ["safety tips", "prevention", "detection", "monitoring", "help"],
                responses: [
                    "💡 General Safety Tips:",
                    "• Install gas detectors (CO, NH₃, CO₂)",
                    "• Keep indoor spaces well-ventilated",
                    "• Service appliances like stoves, heaters, and AC units",
                    "• Avoid burning trash or using unvented stoves indoors",
                    "• Support renewable energy to lower CO₂ emissions",
                    "• Use proper ventilation in farms and industrial areas",
                    "• Store fertilizers and chemicals properly"
                ]
            }
        };
        
        // Chart initialization
        function initChart() {
            const ctx = document.getElementById('pmChart').getContext('2d');
            
            if (pmChart) {
                pmChart.destroy();
            }
            
            pmChart = new Chart(ctx, {
                type: 'line',
                data: {
                    labels: [],
                    datasets: [{
                        label: 'Air Quality (PPM)',
                        data: [],
                        borderColor: '#667eea',
                        backgroundColor: 'rgba(102, 126, 234, 0.2)',
                        tension: 0.4,
                        fill: true,
                        pointRadius: 4,
                        borderWidth: 3
                    }]
                },
                options: {
                    responsive: true,
                    maintainAspectRatio: false,
                    animation: {
                        duration: 1000,
                        easing: 'easeInOutQuart'
                    },
                    plugins: {
                        legend: {
                            labels: {
                                color: getComputedStyle(document.documentElement).getPropertyValue('--text-primary'),
                                font: {
                                    size: 12,
                                    weight: '500'
                                }
                            }
                        }
                    },
                    scales: {
                        x: {
                            ticks: {
                                color: getComputedStyle(document.documentElement).getPropertyValue('--text-secondary'),
                                font: {
                                    size: 11
                                }
                            },
                            grid: {
                                color: getComputedStyle(document.documentElement).getPropertyValue('--border')
                            }
                        },
                        y: {
                            ticks: {
                                color: getComputedStyle(document.documentElement).getPropertyValue('--text-secondary'),
                                font: {
                                    size: 11
                                }
                            },
                            grid: {
                                color: getComputedStyle(document.documentElement).getPropertyValue('--border')
                            }
                        }
                    }
                }
            });
        }
        
        // Update connection status
        function updateConnectionStatus(connected) {
            const statusElement = document.getElementById('connectionStatus');
            const statusIcon = document.getElementById('statusIcon');
            const statusText = document.getElementById('statusText');
            
            if (connected) {
                statusElement.className = 'connection-status connected';
                statusIcon.textContent = '✅';
                statusText.textContent = 'Connected';
            } else {
                statusElement.className = 'connection-status disconnected';
                statusIcon.textContent = '❌';
                statusText.textContent = 'Disconnected';
            }
            isConnected = connected;
        }
        
        // Show alert
        function showAlert(message) {
            const alertCard = document.getElementById('alertCard');
            const alertMessage = document.getElementById('alertMessage');
            alertMessage.textContent = message;
            alertCard.style.display = 'block';
            
            setTimeout(() => {
                alertCard.style.display = 'none';
            }, 10000);
        }
        
        // Update display with sensor data
        function updateDisplay(data) {
            // Update basic readings
            document.getElementById('mq135Ppm').textContent = data.mq135_ppm.toFixed(1);
            document.getElementById('mq135Raw').textContent = data.mq135_raw;
            document.getElementById('temperature').textContent = data.temperature.toFixed(1) + '°C';
            document.getElementById('pressure').textContent = data.pressure.toFixed(1) + ' hPa';
            document.getElementById('altitude').textContent = data.altitude + ' m';
            
            // Update CO status
            const coStatus = document.getElementById('coStatus');
            const coGauge = document.getElementById('coGauge');
            if (data.co_detected) {
                coStatus.textContent = 'DETECTED';
                coStatus.style.color = '#ef4444';
                coGauge.style.width = '100%';
                showAlert('Carbon Monoxide Detected! Ensure proper ventilation.');
            } else {
                coStatus.textContent = 'SAFE';
                coStatus.style.color = '#22c55e';
                coGauge.style.width = '10%';
            }
            
            // Update air quality
            const airQualityStatus = document.getElementById('airQualityStatus');
            const airQualityGauge = document.getElementById('airQualityGauge');
            const aqiNumber = document.getElementById('aqiNumber');
            const aqiLevel = document.getElementById('aqiLevel');
            const aqiIndicator = document.getElementById('aqiIndicator');
            
            airQualityStatus.textContent = data.air_quality_level;
            
            let aqiColor, aqiClass, gaugeWidth;
            switch(data.air_quality_level) {
                case 'Fresh Air':
                    aqiColor = '#22c55e';
                    aqiClass = 'good';
                    gaugeWidth = '25%';
                    aqiNumber.textContent = Math.floor(data.mq135_ppm * 0.5);
                    break;
                case 'Normal Indoor Air':
                    aqiColor = '#f59e0b';
                    aqiClass = 'moderate';
                    gaugeWidth = '50%';
                    aqiNumber.textContent = Math.floor(50 + data.mq135_ppm * 0.3);
                    break;
                case 'Poor Air Quality':
                    aqiColor = '#ef4444';
                    aqiClass = 'unhealthy';
                    gaugeWidth = '75%';
                    aqiNumber.textContent = Math.floor(100 + data.mq135_ppm * 0.4);
                    showAlert('Poor air quality detected: ' + data.air_quality_level);
                    break;
                case 'Hazardous Air':
                    aqiColor = '#7f1d1d';
                    aqiClass = 'unhealthy';
                    gaugeWidth = '100%';
                    aqiNumber.textContent = Math.floor(150 + data.mq135_ppm * 0.5);
                    showAlert('Hazardous air quality detected!');
                    break;
                default:
                    aqiColor = '#6b7280';
                    aqiClass = 'moderate';
                    gaugeWidth = '0%';
                    aqiNumber.textContent = '--';
            }
            
            aqiLevel.textContent = data.air_quality_level;
            aqiLevel.className = 'aqi-level ' + aqiClass;
            aqiIndicator.style.background = aqiColor;
            airQualityGauge.style.width = gaugeWidth;
            
            // Update chart
            if (pmChart) {
                const now = new Date();
                const timeString = now.toLocaleTimeString('en-US', {
                    hour12: false,
                    hour: '2-digit',
                    minute: '2-digit'
                });
                
                pmChart.data.labels.push(timeString);
                pmChart.data.datasets[0].data.push(data.mq135_ppm);
                
                // Keep only last 20 data points
                if (pmChart.data.labels.length > 20) {
                    pmChart.data.labels.shift();
                    pmChart.data.datasets[0].data.shift();
                }
                
                pmChart.update();
            }
            
            // Update timestamp
            const now = new Date();
            document.getElementById('updatedTime').textContent = 'Updated: ' + now.toLocaleTimeString();
        }
        
        // Fetch data from ESP32
        function fetchData() {
            fetch('/data')
                .then(response => {
                    if (!response.ok) throw new Error('Network response was not ok');
                    return response.json();
                })
                .then(data => {
                    updateDisplay(data);
                    updateConnectionStatus(true);
                })
                .catch(error => {
                    console.error('Error fetching data:', error);
                    updateConnectionStatus(false);
                });
        }
        
        // Theme toggle
        function toggleTheme() {
            const body = document.body;
            const icon = document.getElementById('themeIcon');
            const text = document.getElementById('themeText');
            
            if (currentTheme === 'light') {
                body.setAttribute('data-theme', 'dark');
                icon.textContent = '☀️';
                text.textContent = 'Light Mode';
                currentTheme = 'dark';
            } else {
                body.removeAttribute('data-theme');
                icon.textContent = '🌙';
                text.textContent = 'Dark Mode';
                currentTheme = 'light';
            }
            
            // Update chart colors
            if (pmChart) {
                setTimeout(() => {
                    pmChart.options.plugins.legend.labels.color = getComputedStyle(document.documentElement).getPropertyValue('--text-primary');
                    pmChart.options.scales.x.ticks.color = getComputedStyle(document.documentElement).getPropertyValue('--text-secondary');
                    pmChart.options.scales.y.ticks.color = getComputedStyle(document.documentElement).getPropertyValue('--text-secondary');
                    pmChart.options.scales.x.grid.color = getComputedStyle(document.documentElement).getPropertyValue('--border');
                    pmChart.options.scales.y.grid.color = getComputedStyle(document.documentElement).getPropertyValue('--border');
                    pmChart.update();
                }, 100);
            }
        }
        
        // Card highlighting
        function highlightCard(card) {
            document.querySelectorAll('.gas-card').forEach(c => {
                c.style.boxShadow = '';
                c.style.transform = '';
            });
            
            card.style.boxShadow = '0 25px 50px rgba(102, 126, 234, 0.4)';
            card.style.transform = 'translateY(-8px) scale(1.05)';
            
            setTimeout(() => {
                card.style.boxShadow = '';
                card.style.transform = '';
            }, 3000);
        }
        
        // Confetti effect
        function createConfetti() {
            for (let i = 0; i < 30; i++) {
                const confetti = document.createElement('div');
                confetti.style.position = 'fixed';
                confetti.style.width = '10px';
                confetti.style.height = '10px';
                confetti.style.backgroundColor = `hsl(${Math.random() * 360}, 70%, 60%)`;
                confetti.style.left = Math.random() * 100 + 'vw';
                confetti.style.top = '-10px';
                confetti.style.borderRadius = '50%';
                confetti.style.pointerEvents = 'none';
                confetti.style.zIndex = '1000';
                confetti.style.animation = 'confettiFall 3s linear forwards';
                document.body.appendChild(confetti);
                
                setTimeout(() => {
                    confetti.remove();
                }, 3000);
            }
        }
        
        // Create floating particles
        function createFloatingParticles() {
            const particleContainer = document.querySelector('.floating-particles');
            
            for (let i = 0; i < 20; i++) {
                const particle = document.createElement('div');
                particle.className = 'particle';
                particle.style.left = Math.random() * 100 + '%';
                particle.style.top = Math.random() * 100 + '%';
                particle.style.animationDelay = Math.random() * 20 + 's';
                particle.style.animationDuration = (Math.random() * 10 + 10) + 's';
                particleContainer.appendChild(particle);
            }
        }
        
        // AI Chatbot Functions
        function toggleChatbot() {
            const chatbot = document.getElementById('chatbotContainer');
            const toggle = document.getElementById('chatbotToggle');
            
            if (chatbot.style.display === 'none' || chatbot.style.display === '') {
                chatbot.style.display = 'flex';
                toggle.style.display = 'none';
            } else {
                chatbot.style.display = 'none';
                toggle.style.display = 'block';
            }
        }
        
        function handleChatbotKeypress(event) {
            if (event.key === 'Enter') {
                sendChatbotMessage();
            }
        }
        
        function sendChatbotMessage() {
            const input = document.getElementById('chatbotInput');
            const message = input.value.trim();
            
            if (message === '') return;
            
            // Add user message
            addChatbotMessage(message, 'user');
            input.value = '';
            
            // Generate AI response
            setTimeout(() => {
                const response = generateAIResponse(message);
                addChatbotMessage(response, 'bot');
            }, 500);
        }
        
        function addChatbotMessage(text, sender) {
            const messagesContainer = document.getElementById('chatbotMessages');
            const messageDiv = document.createElement('div');
            messageDiv.className = `message ${sender}`;
            messageDiv.textContent = text;
            messagesContainer.appendChild(messageDiv);
            messagesContainer.scrollTop = messagesContainer.scrollHeight;
        }
        
        function generateAIResponse(userMessage) {
            const message = userMessage.toLowerCase();
            
            // Check for specific gas types
            for (const [gas, data] of Object.entries(aiKnowledge)) {
                for (const keyword of data.keywords) {
                    if (message.includes(keyword)) {
                        return data.responses[Math.floor(Math.random() * data.responses.length)];
                    }
                }
            }
            
            // Default response
            return "I can help you with information about Carbon Monoxide (CO), Ammonia (NH₃), Carbon Dioxide (CO₂), Air Quality Index (AQI), and general safety tips. Try asking about specific gases or safety guidelines!";
        }
        
        // Add confetti animation CSS
        const style = document.createElement('style');
        style.textContent = `
            @keyframes confettiFall {
                0% { transform: translateY(-100vh) rotate(0deg); opacity: 1; }
                100% { transform: translateY(100vh) rotate(720deg); opacity: 0; }
            }
        `;
        document.head.appendChild(style);
        
        // Initialize everything
        function initialize() {
            console.log('Initializing ESP32 Air Quality Monitor...');
            
            // Create floating particles
            createFloatingParticles();
            
            // Initialize chart
            setTimeout(() => {
                initChart();
            }, 500);
            
            // Start fetching data
            fetchData();
            setInterval(fetchData, 2000);
            
            // Set initial connection status
            updateConnectionStatus(false);
        }
        
        // Start when page loads
        if (document.readyState === 'loading') {
            document.addEventListener('DOMContentLoaded', initialize);
        } else {
            initialize();
        }
    </script>
</body>
</html>
)rawliteral";

#endif
