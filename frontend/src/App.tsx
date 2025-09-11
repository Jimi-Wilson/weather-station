import LatestWeatherWidget from "@/components/LatestWeatherWidget.tsx";
import WeatherChartsWidget from "@/components/WeatherChartsWidget.tsx";
import { Navbar } from "@/components/Navbar.tsx";

function App() {
    return (
        <div>
            <Navbar />
            <main className="container mx-auto p-4">
                <div className="flex flex-col gap-4">
                    <LatestWeatherWidget />
                    <WeatherChartsWidget />
                </div>
            </main>
        </div>
    )
}

export default App;