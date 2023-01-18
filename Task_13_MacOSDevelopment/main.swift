import Foundation

struct location: Codable {
  let lat: Double
  let lng: Double
}

struct geometry: Codable {
  let location: location
}

struct candidate: Codable {
  let geometry: geometry
  let name: String
}

struct allPlaces: Codable {
  let candidates: [candidate]
}

struct nearestPlace {
  let name: String
  let distance: Double
}

func readLocalJSONFile(forName name: String) -> Data? {
  do {
    if let filePath = Bundle.main.path(forResource: name, ofType: "json") {
      let fileUrl = URL(fileURLWithPath: filePath)
      let data = try Data(contentsOf: fileUrl)
      return data
    }
  } catch {
    print("Read file error: \(error)")
  }
  return nil
}

func parse(jsonData: Data) -> allPlaces? {
  do {
    let decodedData = try JSONDecoder().decode(allPlaces.self, from: jsonData)
    return decodedData
  } catch {
    print("Parse error: \(error)")
  }
  return nil
}

class CoolPlacesNearMe {
  var places: allPlaces

  init?(fileName: String) {
    let jsonData = readLocalJSONFile(forName: fileName)
    if let data = jsonData {
      if let sampleRecordObj = parse(jsonData: data) {
        places = sampleRecordObj
      } else {
        return nil
      }
    } else {
      return nil
    }
  }

  func getDistance(a: location, b: location) -> Double {
    let kEarthRadius: Double = 6_372_795

    let aLatRad = a.lat * .pi / 180
    let aLngRad = a.lng * .pi / 180

    let bLatRad = b.lat * .pi / 180
    let bLngRad = b.lng * .pi / 180

    let ang_distance = acos(
      sin(aLatRad) * sin(bLatRad) + cos(aLatRad) * cos(bLatRad) * cos(bLngRad - aLngRad))

    return kEarthRadius * ang_distance / 1000
  }

  func getNearestPlaces(startPoint: location, radius: Double) -> [nearestPlace] {
    var result: [nearestPlace] = []

    for candidate in places.candidates {
      var distance = getDistance(a: startPoint, b: candidate.geometry.location)
      if distance < radius {
        result.append(nearestPlace(name: candidate.name, distance: distance))
      }
    }

    return result
  }

}

let officeLocation = location(lat: 48.471207, lng: 35.038810)

if let new_place = CoolPlacesNearMe(fileName: "candidates") {
  let result = new_place.getNearestPlaces(startPoint: officeLocation, radius: 2)
  for place in result {
    print("\(place.name) : \(place.distance) km")
  }
}

