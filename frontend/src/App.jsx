import './App.css'
import WeatherDisplay from "./components/WeatherDisplay.jsx";

function App() {

  return (
      <div className="flex flex-col justify-center items-center">
      <div className="flex flex-col justify-start items-start">
          <h1 className="text-5xl mb-20">Matlock Weather Station Dashboard</h1>
          <WeatherDisplay />
      </div>
      </div>

  )
}


export default App
